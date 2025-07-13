/**
 * @file lv_draw_sw_blend_to_rgb888.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_draw_sw_blend_to_rgb888.h"
#if LV_USE_DRAW_SW

#include "lv_draw_sw_blend_private.h"
#include "../../../misc/lv_math.h"
#include "../../../display/lv_display.h"
#include "../../../core/lv_refr.h"
#include "../../../misc/lv_color.h"
#include "../../../stdlib/lv_string.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/


static void /* LV_ATTRIBUTE_FAST_MEM */ rgb888_image_blend(lv_draw_sw_blend_image_dsc_t * dsc,
                                                           const uint8_t dest_px_size,
                                                           uint32_t src_px_size);

static void /* LV_ATTRIBUTE_FAST_MEM */ argb8888_image_blend(lv_draw_sw_blend_image_dsc_t * dsc,
                                                             uint32_t dest_px_size);


static inline void /* LV_ATTRIBUTE_FAST_MEM */ lv_color_24_24_mix(const uint8_t * src, uint8_t * dest, uint8_t mix);


static inline void * /* LV_ATTRIBUTE_FAST_MEM */ drawbuf_next_row(const void * buf, uint32_t stride);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/


/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void LV_ATTRIBUTE_FAST_MEM lv_draw_sw_blend_color_to_rgb888(lv_draw_sw_blend_fill_dsc_t * dsc, uint32_t dest_px_size)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    const lv_opa_t * mask = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;
    int32_t dest_stride = dsc->dest_stride;

    int32_t x;
    int32_t y;

    LV_UNUSED(w);
    LV_UNUSED(h);
    LV_UNUSED(x);
    LV_UNUSED(y);
    LV_UNUSED(opa);
    LV_UNUSED(mask);
    LV_UNUSED(mask_stride);
    LV_UNUSED(dest_stride);

    /*Simple fill*/
    if(mask == NULL && opa >= LV_OPA_MAX) {
        if(dest_px_size == 3) {
            uint8_t * dest_buf_u8 = dsc->dest_buf;
            uint8_t * dest_buf_ori = dsc->dest_buf;
            w *= dest_px_size;

            for(x = 0; x < w; x += 3) {
                dest_buf_u8[x + 0] = dsc->color.blue;
                dest_buf_u8[x + 1] = dsc->color.green;
                dest_buf_u8[x + 2] = dsc->color.red;
            }

            dest_buf_u8 += dest_stride;

            for(y = 1; y < h; y++) {
                lv_memcpy(dest_buf_u8, dest_buf_ori, w);
                dest_buf_u8 += dest_stride;
            }
        }
        if(dest_px_size == 4) {
            uint32_t color32 = lv_color_to_u32(dsc->color);
            uint32_t * dest_buf_u32 = dsc->dest_buf;
            for(y = 0; y < h; y++) {
                for(x = 0; x <= w - 16; x += 16) {
                    dest_buf_u32[x + 0] = color32;
                    dest_buf_u32[x + 1] = color32;
                    dest_buf_u32[x + 2] = color32;
                    dest_buf_u32[x + 3] = color32;

                    dest_buf_u32[x + 4] = color32;
                    dest_buf_u32[x + 5] = color32;
                    dest_buf_u32[x + 6] = color32;
                    dest_buf_u32[x + 7] = color32;

                    dest_buf_u32[x + 8] = color32;
                    dest_buf_u32[x + 9] = color32;
                    dest_buf_u32[x + 10] = color32;
                    dest_buf_u32[x + 11] = color32;

                    dest_buf_u32[x + 12] = color32;
                    dest_buf_u32[x + 13] = color32;
                    dest_buf_u32[x + 14] = color32;
                    dest_buf_u32[x + 15] = color32;
                }
                for(; x < w; x ++) {
                    dest_buf_u32[x] = color32;
                }

                dest_buf_u32 = drawbuf_next_row(dest_buf_u32, dest_stride);
            }
        }
    }
    /*Opacity only*/
    else if(mask == NULL && opa < LV_OPA_MAX) {
        uint32_t color32 = lv_color_to_u32(dsc->color);
        uint8_t * dest_buf = dsc->dest_buf;
        w *= dest_px_size;
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x += dest_px_size) {
                lv_color_24_24_mix((const uint8_t *)&color32, &dest_buf[x], opa);
            }

            dest_buf = drawbuf_next_row(dest_buf, dest_stride);
        }
    }
    /*Masked with full opacity*/
    else if(mask && opa >= LV_OPA_MAX) {
        uint32_t color32 = lv_color_to_u32(dsc->color);
        uint8_t * dest_buf = dsc->dest_buf;
        w *= dest_px_size;

        for(y = 0; y < h; y++) {
            uint32_t mask_x;
            for(x = 0, mask_x = 0; x < w; x += dest_px_size, mask_x++) {
                lv_color_24_24_mix((const uint8_t *)&color32, &dest_buf[x], mask[mask_x]);
            }

            dest_buf += dest_stride;
            mask += mask_stride;
        }
    }
    /*Masked with opacity*/
    else {
        uint32_t color32 = lv_color_to_u32(dsc->color);
        uint8_t * dest_buf = dsc->dest_buf;
        w *= dest_px_size;

        for(y = 0; y < h; y++) {
            uint32_t mask_x;
            for(x = 0, mask_x = 0; x < w; x += dest_px_size, mask_x++) {
                lv_color_24_24_mix((const uint8_t *) &color32, &dest_buf[x], LV_OPA_MIX2(opa, mask[mask_x]));
            }
            dest_buf += dest_stride;
            mask += mask_stride;
        }
    }
}

void LV_ATTRIBUTE_FAST_MEM lv_draw_sw_blend_image_to_rgb888(lv_draw_sw_blend_image_dsc_t * dsc, uint32_t dest_px_size)
{
    switch(dsc->src_color_format) {
        case LV_COLOR_FORMAT_RGB888:
            rgb888_image_blend(dsc, dest_px_size, 3);
            break;
        case LV_COLOR_FORMAT_XRGB8888:
            rgb888_image_blend(dsc, dest_px_size, 4);
            break;
        case LV_COLOR_FORMAT_ARGB8888:
            argb8888_image_blend(dsc, dest_px_size);
            break;

        default:
            LV_LOG_WARN("Not supported source color format");
            break;
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void LV_ATTRIBUTE_FAST_MEM rgb888_image_blend(lv_draw_sw_blend_image_dsc_t * dsc, const uint8_t dest_px_size,
                                                     uint32_t src_px_size)
{
    int32_t w = dsc->dest_w * dest_px_size;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    uint8_t * dest_buf = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;
    const uint8_t * src_buf = dsc->src_buf;
    int32_t src_stride = dsc->src_stride;
    const lv_opa_t * mask_buf = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;

    int32_t dest_x;
    int32_t src_x;
    int32_t y;

    /*Special case*/
    if(mask_buf == NULL && opa >= LV_OPA_MAX) {
        if(src_px_size == dest_px_size) {
            for(y = 0; y < h; y++) {
                lv_memcpy(dest_buf, src_buf, w);
                dest_buf += dest_stride;
                src_buf += src_stride;
            }
        }
        else {
            for(y = 0; y < h; y++) {
                for(dest_x = 0, src_x = 0; dest_x < w; dest_x += dest_px_size, src_x += src_px_size) {
                    dest_buf[dest_x + 0] = src_buf[src_x + 0];
                    dest_buf[dest_x + 1] = src_buf[src_x + 1];
                    dest_buf[dest_x + 2] = src_buf[src_x + 2];
                }
                dest_buf += dest_stride;
                src_buf += src_stride;
            }
        }
    }
    if(mask_buf == NULL && opa < LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            for(dest_x = 0, src_x = 0; dest_x < w; dest_x += dest_px_size, src_x += src_px_size) {
                lv_color_24_24_mix(&src_buf[src_x], &dest_buf[dest_x], opa);
            }
            dest_buf += dest_stride;
            src_buf += src_stride;
        }
    }
    if(mask_buf && opa >= LV_OPA_MAX) {
        uint32_t mask_x;
        for(y = 0; y < h; y++) {
            for(mask_x = 0, dest_x = 0, src_x = 0; dest_x < w; mask_x++, dest_x += dest_px_size, src_x += src_px_size) {
                lv_color_24_24_mix(&src_buf[src_x], &dest_buf[dest_x], mask_buf[mask_x]);
            }
            dest_buf += dest_stride;
            src_buf += src_stride;
            mask_buf += mask_stride;
        }
    }
    if(mask_buf && opa < LV_OPA_MAX) {
        uint32_t mask_x;
        for(y = 0; y < h; y++) {
            for(mask_x = 0, dest_x = 0, src_x = 0; dest_x < w; mask_x++, dest_x += dest_px_size, src_x += src_px_size) {
                lv_color_24_24_mix(&src_buf[src_x], &dest_buf[dest_x], LV_OPA_MIX2(opa, mask_buf[mask_x]));
            }
            dest_buf += dest_stride;
            src_buf += src_stride;
            mask_buf += mask_stride;
        }
    }
}

static void LV_ATTRIBUTE_FAST_MEM argb8888_image_blend(lv_draw_sw_blend_image_dsc_t * dsc, uint32_t dest_px_size)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    uint8_t * dest_buf = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;
    const lv_color32_t * src_buf_c32 = dsc->src_buf;
    int32_t src_stride = dsc->src_stride;
    const lv_opa_t * mask_buf = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;

    int32_t dest_x;
    int32_t src_x;
    int32_t y;

    if(mask_buf == NULL && opa >= LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            for(dest_x = 0, src_x = 0; src_x < w; dest_x += dest_px_size, src_x++) {
                lv_color_24_24_mix((const uint8_t *)&src_buf_c32[src_x], &dest_buf[dest_x], src_buf_c32[src_x].alpha);
            }
            dest_buf += dest_stride;
            src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
        }
    }
    else if(mask_buf == NULL && opa < LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            for(dest_x = 0, src_x = 0; src_x < w; dest_x += dest_px_size, src_x++) {
                lv_color_24_24_mix((const uint8_t *)&src_buf_c32[src_x], &dest_buf[dest_x], LV_OPA_MIX2(src_buf_c32[src_x].alpha, opa));
            }
            dest_buf += dest_stride;
            src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
        }
    }
    else if(mask_buf && opa >= LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            for(dest_x = 0, src_x = 0; src_x < w; dest_x += dest_px_size, src_x++) {
                lv_color_24_24_mix((const uint8_t *)&src_buf_c32[src_x], &dest_buf[dest_x],
                                   LV_OPA_MIX2(src_buf_c32[src_x].alpha, mask_buf[src_x]));
            }
            dest_buf += dest_stride;
            src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
            mask_buf += mask_stride;
        }
    }
    else if(mask_buf && opa < LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            for(dest_x = 0, src_x = 0; src_x < w; dest_x += dest_px_size, src_x++) {
                lv_color_24_24_mix((const uint8_t *)&src_buf_c32[src_x], &dest_buf[dest_x],
                                   LV_OPA_MIX3(src_buf_c32[src_x].alpha, mask_buf[src_x], opa));
            }
            dest_buf += dest_stride;
            src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
            mask_buf += mask_stride;
        }
    }
}

static inline void LV_ATTRIBUTE_FAST_MEM lv_color_24_24_mix(const uint8_t * src, uint8_t * dest, uint8_t mix)
{

    if(mix == 0) return;

    if(mix >= LV_OPA_MAX) {
        dest[0] = src[0];
        dest[1] = src[1];
        dest[2] = src[2];
    }
    else {
        lv_opa_t mix_inv = 255 - mix;
        dest[0] = (uint32_t)((uint32_t)src[0] * mix + dest[0] * mix_inv) >> 8;
        dest[1] = (uint32_t)((uint32_t)src[1] * mix + dest[1] * mix_inv) >> 8;
        dest[2] = (uint32_t)((uint32_t)src[2] * mix + dest[2] * mix_inv) >> 8;
    }
}


static inline void * LV_ATTRIBUTE_FAST_MEM drawbuf_next_row(const void * buf, uint32_t stride)
{
    return (void *)((uint8_t *)buf + stride);
}

#endif /*LV_USE_DRAW_SW*/
