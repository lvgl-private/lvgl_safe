/**
 * @file lv_bin_decoder.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../draw/lv_image_decoder_private.h"
#include "lv_bin_decoder.h"
#include "../../draw/lv_draw_image.h"
#include "../../draw/lv_draw_buf.h"
#include "../../stdlib/lv_string.h"
#include "../../stdlib/lv_sprintf.h"
#include "../../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/

#define DECODER_NAME    "BIN"

#define image_cache_draw_buf_handlers &(LV_GLOBAL_DEFAULT()->image_cache_draw_buf_handlers)

/**********************
 *      TYPEDEFS
 **********************/

/**
 * Data format for compressed image data.
 */

typedef struct _lv_image_compressed_t {
	uint32_t method: 4; /*Compression method, see `lv_image_compress_t`*/
	uint32_t reserved : 28;  /*Reserved to be used later*/
	uint32_t compressed_size;  /*Compressed data size in byte*/
	uint32_t decompressed_size;  /*Decompressed data size in byte*/
	const uint8_t * data; /*Compressed data*/
} lv_image_compressed_t;

typedef struct {
	lv_fs_file_t * f;
	lv_color32_t * palette;
	lv_opa_t * opa;
	lv_image_compressed_t compressed;
	lv_draw_buf_t * decoded;            /*A draw buf to store decoded image*/
	lv_draw_buf_t * decompressed;       /*Decompressed data could be used directly, thus must also be draw buf*/
	lv_draw_buf_t c_array;              /*An C-array image that need to be converted to a draw buf*/
	lv_draw_buf_t * decoded_partial;    /*A draw buf for decoded image via get_area_cb*/
} decoder_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static decoder_data_t * get_decoder_data(lv_image_decoder_dsc_t * dsc);
static void free_decoder_data(lv_image_decoder_dsc_t * dsc);

static lv_fs_res_t fs_read_file_at(lv_fs_file_t * f, uint32_t pos, void * buff, uint32_t btr, uint32_t * br);


/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the lvgl binary image decoder module
 */
void lv_bin_decoder_init(void)
{
	lv_image_decoder_t * decoder;

	decoder = lv_image_decoder_create();
	LV_ASSERT_MALLOC(decoder);
	if(decoder == NULL) {
		LV_LOG_WARN("Out of memory");
		return;
	}

	lv_image_decoder_set_info_cb(decoder, lv_bin_decoder_info);
	lv_image_decoder_set_open_cb(decoder, lv_bin_decoder_open);
	lv_image_decoder_set_get_area_cb(decoder, lv_bin_decoder_get_area);
	lv_image_decoder_set_close_cb(decoder, lv_bin_decoder_close);

	decoder->name = DECODER_NAME;
}

lv_result_t lv_bin_decoder_info(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc, lv_image_header_t * header)
{
	LV_UNUSED(decoder); /*Unused*/

	const void * src = dsc->src;
	lv_image_src_t src_type = dsc->src_type;

	if(src_type == LV_IMAGE_SRC_VARIABLE) {
		lv_image_dsc_t * image = (lv_image_dsc_t *)src;
		lv_memcpy(header, &image->header, sizeof(lv_image_header_t));
	}
	else if(src_type == LV_IMAGE_SRC_FILE) {
		/*Support only "*.bin" files*/
		if(lv_strcmp(lv_fs_get_ext(src), "bin")) return LV_RESULT_INVALID;

		lv_fs_res_t res;
		uint32_t rn;
		res = lv_fs_read(&dsc->file, header, sizeof(lv_image_header_t), &rn);

		if(res != LV_FS_RES_OK || rn != sizeof(lv_image_header_t)) {
			LV_LOG_WARN("Read file header failed: %d with len: %" LV_PRIu32 ", expected: %zu", res, rn, sizeof(lv_image_header_t));
			return LV_RESULT_INVALID;
		}

		/**
		 * @todo
		 * This is a temp backward compatibility solution after adding
		 * magic in image header.
		 */
		if(header->magic != LV_IMAGE_HEADER_MAGIC) {
			LV_LOG_WARN("Legacy bin image detected: %s", (char *)src);
			header->cf = header->magic;
			header->magic = LV_IMAGE_HEADER_MAGIC;
		}

		/*File is always read to buf, thus data can be modified.*/
		header->flags |= LV_IMAGE_FLAGS_MODIFIABLE;
	}
	else if(src_type == LV_IMAGE_SRC_SYMBOL) {
		/*The size depend on the font but it is unknown here. It should be handled outside of the
		 *function*/
		header->w = 1;
		header->h = 1;
		/*Symbols always have transparent parts. Important because of cover check in the draw
		 *function. The actual value doesn't matter because lv_draw_label will draw it*/
		header->cf = LV_COLOR_FORMAT_A8;
	}
	else {
		LV_LOG_WARN("Image get info found unknown src type");
		return LV_RESULT_INVALID;
	}

	if(header->cf == LV_COLOR_FORMAT_UNKNOWN) {
		LV_LOG_WARN("Image color format is unknown");
		return LV_RESULT_INVALID;
	}

	/*For backward compatibility, all images are not premultiplied for now.*/
	if(header->magic != LV_IMAGE_HEADER_MAGIC) {
		header->flags &= ~LV_IMAGE_FLAGS_PREMULTIPLIED;
	}

	return LV_RESULT_OK;
}

/**
 * Decode an image from a binary file
 * @param decoder pointer to the decoder
 * @param dsc     pointer to the decoder descriptor
 * @return LV_RESULT_OK: no error; LV_RESULT_INVALID: can't open the image
 */
lv_result_t lv_bin_decoder_open(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc)
{
	LV_UNUSED(decoder);

	lv_result_t res = LV_RESULT_INVALID;
	lv_fs_res_t fs_res = LV_FS_RES_UNKNOWN;
	bool use_directly = false; /*If the image is already decoded and can be used directly*/

	/*Open the file if it's a file*/
	if(dsc->src_type == LV_IMAGE_SRC_FILE) {
		/*Support only "*.bin" files*/
		if(lv_strcmp(lv_fs_get_ext(dsc->src), "bin")) return LV_RESULT_INVALID;

		/*If the file was open successfully save the file descriptor*/
		decoder_data_t * decoder_data = get_decoder_data(dsc);
		if(decoder_data == NULL) {
			return LV_RESULT_INVALID;
		}

		dsc->user_data = decoder_data;
		lv_fs_file_t * f = lv_malloc(sizeof(*f));
		if(f == NULL) {
			free_decoder_data(dsc);
			return LV_RESULT_INVALID;
		}

		fs_res = lv_fs_open(f, dsc->src, LV_FS_MODE_RD);
		if(fs_res != LV_FS_RES_OK) {
			LV_LOG_WARN("Open file failed: %d", fs_res);
			lv_free(f);
			free_decoder_data(dsc);
			return LV_RESULT_INVALID;
		}

		decoder_data->f = f;    /*Now free_decoder_data will take care of the file*/

		lv_color_format_t cf = dsc->header.cf;
		/* decode them in get_area_cb */
		res = LV_RESULT_OK;
	}

	else if(dsc->src_type == LV_IMAGE_SRC_VARIABLE) {
		/*The variables should have valid data*/
		lv_image_dsc_t * image = (lv_image_dsc_t *)dsc->src;
		if(image->data == NULL) {
			return LV_RESULT_INVALID;
		}

		lv_color_format_t cf = image->header.cf;

		/*In case of uncompressed formats the image stored in the ROM/RAM.
		 *So simply give its pointer*/

		decoder_data_t * decoder_data = get_decoder_data(dsc);
		lv_draw_buf_t * decoded;
		if(image->header.flags & LV_IMAGE_FLAGS_ALLOCATED) {
			decoded = (lv_draw_buf_t *)image;
			res = LV_RESULT_OK;
		}
		else {
			decoded = &decoder_data->c_array;
			if(image->header.stride == 0) {
				/*If image doesn't have stride, treat it as lvgl v8 legacy image format*/
				lv_image_dsc_t tmp = *image;
				tmp.header.stride = (tmp.header.w * lv_color_format_get_bpp(cf) + 7) >> 3;
				res = lv_draw_buf_from_image(decoded, &tmp);
			}
			else
				res = lv_draw_buf_from_image(decoded, image);
		}

		if(res == LV_RESULT_OK) {
			dsc->decoded = decoded;

			if(decoded->header.stride == 0) {
				/*Use the auto calculated value from decoder_info callback*/
				decoded->header.stride = dsc->header.stride;
			}

			use_directly = true; /*A variable image that can be used directly.*/
		}

	}

	if(res != LV_RESULT_OK) {
		free_decoder_data(dsc);
		return res;
	}

	if(dsc->decoded == NULL) return LV_RESULT_OK; /*Need to read via get_area_cb*/

	lv_draw_buf_t * decoded = (lv_draw_buf_t *)dsc->decoded;
	if(dsc->header.flags & LV_IMAGE_FLAGS_PREMULTIPLIED) {
		lv_draw_buf_set_flag(decoded, LV_IMAGE_FLAGS_PREMULTIPLIED);
	}

	lv_draw_buf_t * adjusted = lv_image_decoder_post_process(dsc, decoded);
	if(adjusted == NULL) {
		free_decoder_data(dsc);
		return LV_RESULT_INVALID;
	}

	/*The adjusted draw buffer is newly allocated.*/
	if(adjusted != decoded) {
		use_directly = false; /*Cannot use original image directly*/
		free_decoder_data(dsc);
		decoder_data_t * decoder_data = get_decoder_data(dsc);
		decoder_data->decoded = adjusted; /*Now this new buffer need to be free'd on decoder close*/
	}
	dsc->decoded = adjusted;

	/* Copy user flags to the decoded image */
	if(dsc->header.flags & LV_IMAGE_FLAGS_USER_MASK) {
		lv_draw_buf_set_flag((lv_draw_buf_t *)dsc->decoded, dsc->header.flags & LV_IMAGE_FLAGS_USER_MASK);
	}

	/*Do not put image to cache if it can be used directly.*/
	if(use_directly || dsc->args.no_cache) {
		if(dsc->args.flush_cache && use_directly) {
			dsc->args.flush_cache = false;
		}
		return LV_RESULT_OK;
	}

	/*If the image cache is disabled, just return the decoded image*/
	if(!lv_image_cache_is_enabled()) return LV_RESULT_OK;

	/*Add it to cache*/
	lv_image_cache_data_t search_key;
	search_key.src_type = dsc->src_type;
	search_key.src = dsc->src;
	search_key.slot.size = dsc->decoded->data_size;

	lv_cache_entry_t * cache_entry = lv_image_decoder_add_to_cache(decoder, &search_key, dsc->decoded, dsc->user_data);
	if(cache_entry == NULL) {
		free_decoder_data(dsc);
		return LV_RESULT_INVALID;
	}
	dsc->cache_entry = cache_entry;
	decoder_data_t * decoder_data = get_decoder_data(dsc);
	decoder_data->decoded = NULL; /*Cache will take care of it*/

	return LV_RESULT_OK;
}

void lv_bin_decoder_close(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc)
{
	LV_UNUSED(decoder); /*Unused*/

	decoder_data_t * decoder_data = dsc->user_data;
	if(decoder_data && decoder_data->decoded_partial) {
		lv_draw_buf_destroy(decoder_data->decoded_partial);
		decoder_data->decoded_partial = NULL;
	}

	free_decoder_data(dsc);
}

lv_result_t lv_bin_decoder_get_area(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc,
		const lv_area_t * full_area, lv_area_t * decoded_area)
{
	LV_UNUSED(decoder); /*Unused*/

	lv_color_format_t cf = dsc->header.cf;
	/*Check if cf is supported*/

	bool supported = cf == LV_COLOR_FORMAT_ARGB8888  \
			|| cf == LV_COLOR_FORMAT_XRGB8888  \
			|| cf == LV_COLOR_FORMAT_RGB888    \
			|| cf == LV_COLOR_FORMAT_RGB565;
	if(!supported) {
		LV_LOG_WARN("CF: %d is not supported", cf);
		return LV_RESULT_INVALID;
	}

	lv_fs_res_t res = LV_FS_RES_UNKNOWN;
	decoder_data_t * decoder_data = dsc->user_data;
	if(decoder_data == NULL) {
		LV_LOG_ERROR("Unexpected null decoder data");
		return LV_RESULT_INVALID;
	}

	lv_fs_file_t * f = decoder_data->f;
	uint32_t bpp = lv_color_format_get_bpp(cf);
	int32_t w_px = lv_area_get_width(full_area);
	uint8_t * img_data = NULL;
	lv_draw_buf_t * decoded = NULL;
	uint32_t offset = dsc->src_type == LV_IMAGE_SRC_FILE ? sizeof(lv_image_header_t) : 0;   /*Skip the image header*/

	/*We only support read line by line for now*/
	if(decoded_area->y1 == LV_COORD_MIN) {
		/*Indexed image is converted to ARGB888*/
		lv_color_format_t cf_decoded = cf;

		decoded = lv_draw_buf_reshape(decoder_data->decoded_partial, cf_decoded, w_px, 1, LV_STRIDE_AUTO);
		if(decoded == NULL) {
			if(decoder_data->decoded_partial != NULL) {
				lv_draw_buf_destroy(decoder_data->decoded_partial);
				decoder_data->decoded_partial = NULL;
			}
			decoded = lv_draw_buf_create_ex(image_cache_draw_buf_handlers, w_px, 1, cf_decoded, LV_STRIDE_AUTO);
			if(decoded == NULL) return LV_RESULT_INVALID;
			decoder_data->decoded_partial = decoded; /*Free on decoder close*/
		}
		*decoded_area = *full_area;
		decoded_area->y2 = decoded_area->y1;
	}
	else {
		decoded_area->y1++;
		decoded_area->y2++;
		decoded = decoder_data->decoded_partial; /*Already allocated*/
	}

	img_data = decoded->data; /*Get the buffer to operate on*/

	if(decoded_area->y1 > full_area->y2) {
		return LV_RESULT_INVALID;
	}

	uint32_t len = (w_px * bpp) / 8;
	offset += decoded_area->y1 * dsc->header.stride;
	offset += decoded_area->x1 * bpp / 8; /*Move to x1*/
	res = fs_read_file_at(f, offset, img_data, len, NULL);
	if(res != LV_FS_RES_OK) {
		return LV_RESULT_INVALID;
	}

	dsc->decoded = decoded; /*Return decoded image*/
	return LV_RESULT_OK;

	return LV_RESULT_INVALID;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static decoder_data_t * get_decoder_data(lv_image_decoder_dsc_t * dsc)
{
	decoder_data_t * data = dsc->user_data;
	if(data == NULL) {
		data = lv_malloc_zeroed(sizeof(decoder_data_t));
		LV_ASSERT_MALLOC(data);
		if(data == NULL) {
			LV_LOG_ERROR("Out of memory");
			return NULL;
		}

		dsc->user_data = data;
	}

	return data;
}

static void free_decoder_data(lv_image_decoder_dsc_t * dsc)
{
	decoder_data_t * decoder_data = dsc->user_data;
	if(decoder_data == NULL) return;

	if(decoder_data->f) {
		lv_fs_close(decoder_data->f);
		lv_free(decoder_data->f);
	}

	if(decoder_data->decoded) lv_draw_buf_destroy(decoder_data->decoded);
	if(decoder_data->decompressed) lv_draw_buf_destroy(decoder_data->decompressed);
	lv_free(decoder_data->palette);
	lv_free(decoder_data);
	dsc->user_data = NULL;
}

static lv_fs_res_t fs_read_file_at(lv_fs_file_t * f, uint32_t pos, void * buff, uint32_t btr, uint32_t * br)
{
	lv_fs_res_t res;
	if(br) *br = 0;

	res = lv_fs_seek(f, pos, LV_FS_SEEK_SET);
	if(res != LV_FS_RES_OK) {
		return res;
	}

	res |= lv_fs_read(f, buff, btr, br);
	if(res != LV_FS_RES_OK) {
		return res;
	}

	return LV_FS_RES_OK;
}
