/**
 * @file lv_draw_sw_blend.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../../misc/lv_area_private.h"
#include "lv_draw_sw_blend_private.h"
#include "../../lv_draw_private.h"
#include "../lv_draw_sw.h"
#include "lv_draw_sw_blend_to_rgb565.h"
#include "lv_draw_sw_blend_to_rgb888.h"

#if LV_USE_DRAW_SW

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static inline void /* LV_ATTRIBUTE_FAST_MEM */ lv_draw_sw_blend_color(lv_color_format_t layer_cf,
                                                                      lv_draw_sw_blend_fill_dsc_t * fill_dsc);

static inline void /* LV_ATTRIBUTE_FAST_MEM */ lv_draw_sw_blend_image(lv_color_format_t layer_cf,
                                                                      lv_draw_sw_blend_image_dsc_t * image_dsc);


/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_sw_blend(lv_draw_task_t * t, const lv_draw_sw_blend_dsc_t * blend_dsc)
{
    /*Do not draw transparent things*/
    if(blend_dsc->opa <= LV_OPA_MIN) return;
    if(blend_dsc->mask_buf && blend_dsc->mask_res == LV_DRAW_SW_MASK_RES_TRANSP) return;

    lv_area_t blend_area;
    if(!lv_area_intersect(&blend_area, blend_dsc->blend_area, &t->clip_area)) return;

    LV_PROFILER_DRAW_BEGIN;
    lv_layer_t * layer = t->target_layer;
    uint32_t layer_stride_byte = layer->draw_buf->header.stride;

    lv_draw_sw_blend_handler_t handler = lv_draw_sw_get_blend_handler(layer->color_format);
    if(handler) {
        handler(t, blend_dsc);
        LV_PROFILER_DRAW_END;
        return;
    }

    if(blend_dsc->src_buf == NULL) {
        lv_draw_sw_blend_fill_dsc_t fill_dsc;
        fill_dsc.dest_w = lv_area_get_width(&blend_area);
        fill_dsc.dest_h = lv_area_get_height(&blend_area);
        fill_dsc.dest_stride = layer_stride_byte;
        fill_dsc.opa = blend_dsc->opa;
        fill_dsc.color = blend_dsc->color;
        fill_dsc.mask_stride = 0;

        if(blend_dsc->mask_buf == NULL) fill_dsc.mask_buf = NULL;
        else if(blend_dsc->mask_res == LV_DRAW_SW_MASK_RES_FULL_COVER) fill_dsc.mask_buf = NULL;
        else fill_dsc.mask_buf = blend_dsc->mask_buf;


        fill_dsc.relative_area  = blend_area;
        lv_area_move(&fill_dsc.relative_area, -layer->buf_area.x1, -layer->buf_area.y1);
        fill_dsc.dest_buf = lv_draw_layer_go_to_xy(layer, blend_area.x1 - layer->buf_area.x1,
                                                   blend_area.y1 - layer->buf_area.y1);
        if(fill_dsc.mask_buf) {
            fill_dsc.mask_stride = blend_dsc->mask_stride == 0  ? lv_area_get_width(blend_dsc->mask_area) : blend_dsc->mask_stride;
            fill_dsc.mask_buf += fill_dsc.mask_stride * (blend_area.y1 - blend_dsc->mask_area->y1) +
                                 (blend_area.x1 - blend_dsc->mask_area->x1);
        }

        lv_draw_sw_blend_color(layer->color_format, &fill_dsc);
    }
    else {
        if(!lv_area_intersect(&blend_area, &blend_area, blend_dsc->src_area)) {
            LV_PROFILER_DRAW_END;
            return;
        }

        if(blend_dsc->mask_area && !lv_area_intersect(&blend_area, &blend_area, blend_dsc->mask_area)) {
            LV_PROFILER_DRAW_END;
            return;
        }

        lv_draw_sw_blend_image_dsc_t image_dsc;
        image_dsc.dest_w = lv_area_get_width(&blend_area);
        image_dsc.dest_h = lv_area_get_height(&blend_area);
        image_dsc.dest_stride = layer_stride_byte;

        image_dsc.opa = blend_dsc->opa;
        image_dsc.blend_mode = blend_dsc->blend_mode;
        image_dsc.src_stride = blend_dsc->src_stride;
        image_dsc.src_color_format = blend_dsc->src_color_format;

        const uint8_t * src_buf = blend_dsc->src_buf;
        uint32_t src_px_size = lv_color_format_get_bpp(blend_dsc->src_color_format);
        src_buf += image_dsc.src_stride * (blend_area.y1 - blend_dsc->src_area->y1);
        src_buf += ((blend_area.x1 - blend_dsc->src_area->x1) * src_px_size) >> 3;
        image_dsc.src_buf = src_buf;
        image_dsc.mask_stride = 0;

        if(blend_dsc->mask_buf == NULL) image_dsc.mask_buf = NULL;
        else if(blend_dsc->mask_res == LV_DRAW_SW_MASK_RES_FULL_COVER) image_dsc.mask_buf = NULL;
        else image_dsc.mask_buf = blend_dsc->mask_buf;

        if(image_dsc.mask_buf) {
            LV_ASSERT_NULL(blend_dsc->mask_area);
            image_dsc.mask_buf = blend_dsc->mask_buf;
            image_dsc.mask_stride = blend_dsc->mask_stride ? blend_dsc->mask_stride : lv_area_get_width(blend_dsc->mask_area);
            image_dsc.mask_buf += image_dsc.mask_stride * (blend_area.y1 - blend_dsc->mask_area->y1) +
                                  (blend_area.x1 - blend_dsc->mask_area->x1);
        }

        image_dsc.relative_area  = blend_area;
        lv_area_move(&image_dsc.relative_area, -layer->buf_area.x1, -layer->buf_area.y1);

        image_dsc.src_area  = *blend_dsc->src_area;
        lv_area_move(&image_dsc.src_area, -layer->buf_area.x1, -layer->buf_area.y1);

        image_dsc.dest_buf = lv_draw_layer_go_to_xy(layer, blend_area.x1 - layer->buf_area.x1,
                                                    blend_area.y1 - layer->buf_area.y1);

        lv_draw_sw_blend_image(layer->color_format, &image_dsc);
    }
    LV_PROFILER_DRAW_END;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static inline void LV_ATTRIBUTE_FAST_MEM lv_draw_sw_blend_color(lv_color_format_t layer_cf,
                                                                lv_draw_sw_blend_fill_dsc_t * fill_dsc)
{
    switch(layer_cf) {
        case LV_COLOR_FORMAT_RGB565:
            lv_draw_sw_blend_color_to_rgb565(fill_dsc);
            break;
        case LV_COLOR_FORMAT_RGB888:
            lv_draw_sw_blend_color_to_rgb888(fill_dsc, 3);
            break;
        case LV_COLOR_FORMAT_XRGB8888:
            lv_draw_sw_blend_color_to_rgb888(fill_dsc, 4);
            break;
    }
}

static inline void LV_ATTRIBUTE_FAST_MEM lv_draw_sw_blend_image(lv_color_format_t layer_cf,
                                                                lv_draw_sw_blend_image_dsc_t * image_dsc)
{
    switch(layer_cf) {
        case LV_COLOR_FORMAT_RGB565:
            lv_draw_sw_blend_image_to_rgb565(image_dsc);
            break;
        case LV_COLOR_FORMAT_RGB888:
            lv_draw_sw_blend_image_to_rgb888(image_dsc, 3);
            break;
        case LV_COLOR_FORMAT_XRGB8888:
            lv_draw_sw_blend_image_to_rgb888(image_dsc, 4);
            break;
        default:
            break;
    }
}

#endif
