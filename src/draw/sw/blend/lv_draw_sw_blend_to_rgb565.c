/**
 * @file lv_draw_sw_blend_to_rgb565.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_draw_sw_blend_to_rgb565.h"
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

static void /* LV_ATTRIBUTE_FAST_MEM */ rgb565_image_blend(lv_draw_sw_blend_image_dsc_t * dsc);

static void /* LV_ATTRIBUTE_FAST_MEM */ argb8888_image_blend(lv_draw_sw_blend_image_dsc_t * dsc);

static inline uint16_t /* LV_ATTRIBUTE_FAST_MEM */ lv_color_24_16_mix(const uint8_t * c1, uint16_t c2, uint8_t mix);

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

/**
 * Fill an area with a color.
 * Supports normal fill, fill with opacity, fill with mask, and fill with mask and opacity.
 * dest_buf and color have native color depth. (RGB565, RGB888, XRGB8888)
 * The background (dest_buf) cannot have alpha channel
 * @param dest_buf
 * @param dest_area
 * @param dest_stride
 * @param color
 * @param opa
 * @param mask
 * @param mask_stride
 */
void LV_ATTRIBUTE_FAST_MEM lv_draw_sw_blend_color_to_rgb565(lv_draw_sw_blend_fill_dsc_t * dsc)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    uint16_t color16 = lv_color_to_u16(dsc->color);
    lv_opa_t opa = dsc->opa;
    const lv_opa_t * mask = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;
    uint16_t * dest_buf_u16 = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;

    int32_t x;
    int32_t y;

    LV_UNUSED(w);
    LV_UNUSED(h);
    LV_UNUSED(x);
    LV_UNUSED(y);
    LV_UNUSED(opa);
    LV_UNUSED(mask);
    LV_UNUSED(color16);
    LV_UNUSED(mask_stride);
    LV_UNUSED(dest_stride);
    LV_UNUSED(dest_buf_u16);

    /*Simple fill*/
    if(mask == NULL && opa >= LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            uint16_t * dest_end_final = dest_buf_u16 + w;
            uint32_t * dest_end_mid = (uint32_t *)((uint16_t *) dest_buf_u16 + ((w - 1) & ~(0xF)));
            if((lv_uintptr_t)&dest_buf_u16[0] & 0x3) {
                dest_buf_u16[0] = color16;
                dest_buf_u16++;
            }

            uint32_t c32 = (uint32_t)color16 + ((uint32_t)color16 << 16);
            uint32_t * dest32 = (uint32_t *)dest_buf_u16;
            while(dest32 < dest_end_mid) {
                dest32[0] = c32;
                dest32[1] = c32;
                dest32[2] = c32;
                dest32[3] = c32;
                dest32[4] = c32;
                dest32[5] = c32;
                dest32[6] = c32;
                dest32[7] = c32;
                dest32 += 8;
            }

            dest_buf_u16 = (uint16_t *)dest32;

            while(dest_buf_u16 < dest_end_final) {
                *dest_buf_u16 = color16;
                dest_buf_u16++;
            }

            dest_buf_u16 = drawbuf_next_row(dest_buf_u16, dest_stride);
            dest_buf_u16 -= w;
        }

    }
    /*Opacity only*/
    else if(mask == NULL && opa < LV_OPA_MAX) {
        uint32_t last_dest32_color = dest_buf_u16[0] + 1; /*Set to value which is not equal to the first pixel*/
        uint32_t last_res32_color = 0;

        for(y = 0; y < h; y++) {
            x = 0;
            if((lv_uintptr_t)&dest_buf_u16[0] & 0x3) {
                dest_buf_u16[0] = lv_color_16_16_mix(color16, dest_buf_u16[0], opa);
                x = 1;
            }

            for(; x < w - 2; x += 2) {
                if(dest_buf_u16[x] != dest_buf_u16[x + 1]) {
                    dest_buf_u16[x + 0] = lv_color_16_16_mix(color16, dest_buf_u16[x + 0], opa);
                    dest_buf_u16[x + 1] = lv_color_16_16_mix(color16, dest_buf_u16[x + 1], opa);
                }
                else {
                    volatile uint32_t * dest32 = (uint32_t *)&dest_buf_u16[x];
                    if(last_dest32_color == *dest32) {
                        *dest32 = last_res32_color;
                    }
                    else {
                        last_dest32_color =  *dest32;

                        dest_buf_u16[x] = lv_color_16_16_mix(color16, dest_buf_u16[x + 0], opa);
                        dest_buf_u16[x + 1] = dest_buf_u16[x];

                        last_res32_color = *dest32;
                    }
                }
            }

            for(; x < w ; x++) {
                dest_buf_u16[x] = lv_color_16_16_mix(color16, dest_buf_u16[x], opa);
            }
            dest_buf_u16 = drawbuf_next_row(dest_buf_u16, dest_stride);
        }
    }
    /*Masked with full opacity*/
    else if(mask && opa >= LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            x = 0;
            if((lv_uintptr_t)(mask) & 0x1) {
                dest_buf_u16[x] = lv_color_16_16_mix(color16, dest_buf_u16[x], mask[x]);
                x++;
            }

            for(; x <= w - 2; x += 2) {
                uint16_t mask16 = *((uint16_t *)&mask[x]);
                if(mask16 == 0xFFFF) {
                    dest_buf_u16[x + 0] = color16;
                    dest_buf_u16[x + 1] = color16;
                }
                else if(mask16 != 0) {
                    dest_buf_u16[x + 0] = lv_color_16_16_mix(color16, dest_buf_u16[x + 0], mask[x + 0]);
                    dest_buf_u16[x + 1] = lv_color_16_16_mix(color16, dest_buf_u16[x + 1], mask[x + 1]);
                }
            }

            for(; x < w ; x++) {
                dest_buf_u16[x] = lv_color_16_16_mix(color16, dest_buf_u16[x], mask[x]);
            }
            dest_buf_u16 = drawbuf_next_row(dest_buf_u16, dest_stride);
            mask += mask_stride;
        }
    }
    /*Masked with opacity*/
    else if(mask && opa < LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                dest_buf_u16[x] = lv_color_16_16_mix(color16, dest_buf_u16[x], LV_OPA_MIX2(mask[x], opa));
            }
            dest_buf_u16 = drawbuf_next_row(dest_buf_u16, dest_stride);
            mask += mask_stride;
        }
    }
}

void LV_ATTRIBUTE_FAST_MEM lv_draw_sw_blend_image_to_rgb565(lv_draw_sw_blend_image_dsc_t * dsc)
{
    switch(dsc->src_color_format) {
        case LV_COLOR_FORMAT_RGB565:
            rgb565_image_blend(dsc);
            break;
        case LV_COLOR_FORMAT_ARGB8888:
            argb8888_image_blend(dsc);
            break;
        default:
            LV_LOG_WARN("Not supported source color format");
            break;
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void LV_ATTRIBUTE_FAST_MEM rgb565_image_blend(lv_draw_sw_blend_image_dsc_t * dsc)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    uint16_t * dest_buf_u16 = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;
    const uint16_t * src_buf_u16 = dsc->src_buf;
    int32_t src_stride = dsc->src_stride;
    const lv_opa_t * mask_buf = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;

    int32_t x;
    int32_t y;

    if(mask_buf == NULL && opa >= LV_OPA_MAX) {
        uint32_t line_in_bytes = w * 2;
        for(y = 0; y < h; y++) {
            lv_memcpy(dest_buf_u16, src_buf_u16, line_in_bytes);
            dest_buf_u16 = drawbuf_next_row(dest_buf_u16, dest_stride);
            src_buf_u16 = drawbuf_next_row(src_buf_u16, src_stride);
        }
    }
    else if(mask_buf == NULL && opa < LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                dest_buf_u16[x] = lv_color_16_16_mix(src_buf_u16[x], dest_buf_u16[x], opa);
            }
            dest_buf_u16 = drawbuf_next_row(dest_buf_u16, dest_stride);
            src_buf_u16 = drawbuf_next_row(src_buf_u16, src_stride);
        }
    }
    else if(mask_buf && opa >= LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                dest_buf_u16[x] = lv_color_16_16_mix(src_buf_u16[x], dest_buf_u16[x], mask_buf[x]);
            }
            dest_buf_u16 = drawbuf_next_row(dest_buf_u16, dest_stride);
            src_buf_u16 = drawbuf_next_row(src_buf_u16, src_stride);
            mask_buf += mask_stride;
        }
    }
}

static void LV_ATTRIBUTE_FAST_MEM argb8888_image_blend(lv_draw_sw_blend_image_dsc_t * dsc)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    uint16_t * dest_buf_u16 = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;
    const uint8_t * src_buf_u8 = dsc->src_buf;
    int32_t src_stride = dsc->src_stride;
    const lv_opa_t * mask_buf = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;

    int32_t dest_x;
    int32_t src_x;
    int32_t y;

    if(mask_buf == NULL && opa >= LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            for(dest_x = 0, src_x = 0; dest_x < w; dest_x++, src_x += 4) {
                dest_buf_u16[dest_x] = lv_color_24_16_mix(&src_buf_u8[src_x], dest_buf_u16[dest_x], src_buf_u8[src_x + 3]);
            }
            dest_buf_u16 = drawbuf_next_row(dest_buf_u16, dest_stride);
            src_buf_u8 += src_stride;
        }
    }
    else if(mask_buf == NULL && opa < LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            for(dest_x = 0, src_x = 0; dest_x < w; dest_x++, src_x += 4) {
                dest_buf_u16[dest_x] = lv_color_24_16_mix(&src_buf_u8[src_x], dest_buf_u16[dest_x], LV_OPA_MIX2(src_buf_u8[src_x + 3],
                                                                                                                opa));
            }
            dest_buf_u16 = drawbuf_next_row(dest_buf_u16, dest_stride);
            src_buf_u8 += src_stride;
        }
    }
    else if(mask_buf && opa >= LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            for(dest_x = 0, src_x = 0; dest_x < w; dest_x++, src_x += 4) {
                dest_buf_u16[dest_x] = lv_color_24_16_mix(&src_buf_u8[src_x], dest_buf_u16[dest_x],
                                                          LV_OPA_MIX2(src_buf_u8[src_x + 3], mask_buf[dest_x]));
            }
            dest_buf_u16 = drawbuf_next_row(dest_buf_u16, dest_stride);
            src_buf_u8 += src_stride;
            mask_buf += mask_stride;
        }
    }
    else if(mask_buf && opa < LV_OPA_MAX) {
        for(y = 0; y < h; y++) {
            for(dest_x = 0, src_x = 0; dest_x < w; dest_x++, src_x += 4) {
                dest_buf_u16[dest_x] = lv_color_24_16_mix(&src_buf_u8[src_x], dest_buf_u16[dest_x],
                                                          LV_OPA_MIX3(src_buf_u8[src_x + 3], mask_buf[dest_x], opa));
            }
            dest_buf_u16 = drawbuf_next_row(dest_buf_u16, dest_stride);
            src_buf_u8 += src_stride;
            mask_buf += mask_stride;
        }
    }
}


static inline uint16_t LV_ATTRIBUTE_FAST_MEM lv_color_24_16_mix(const uint8_t * c1, uint16_t c2, uint8_t mix)
{
    if(mix == 0) {
        return c2;
    }
    else if(mix == 255) {
        return ((c1[2] & 0xF8) << 8)  + ((c1[1] & 0xFC) << 3) + ((c1[0] & 0xF8) >> 3);
    }
    else {
        lv_opa_t mix_inv = 255 - mix;

        return ((((c1[2] >> 3) * mix + ((c2 >> 11) & 0x1F) * mix_inv) << 3) & 0xF800) +
               ((((c1[1] >> 2) * mix + ((c2 >> 5) & 0x3F) * mix_inv) >> 3) & 0x07E0) +
               (((c1[0] >> 3) * mix + (c2 & 0x1F) * mix_inv) >> 8);
    }
}

static inline void * LV_ATTRIBUTE_FAST_MEM drawbuf_next_row(const void * buf, uint32_t stride)
{
    return (void *)((uint8_t *)buf + stride);
}


#endif /*LV_USE_DRAW_SW*/
