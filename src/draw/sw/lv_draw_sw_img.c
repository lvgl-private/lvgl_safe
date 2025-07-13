/**
 * @file lv_draw_sw_img.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../misc/lv_area_private.h"
#include "blend/lv_draw_sw_blend_private.h"
#include "../lv_image_decoder_private.h"
#include "../lv_draw_image_private.h"
#include "../lv_draw_private.h"
#include "lv_draw_sw.h"
#if LV_USE_DRAW_SW

#include "../../display/lv_display.h"
#include "../../display/lv_display_private.h"
#include "../../misc/lv_log.h"
#include "../../core/lv_refr_private.h"
#include "../../stdlib/lv_mem.h"
#include "../../misc/lv_math.h"
#include "../../misc/lv_color.h"
#include "../../stdlib/lv_string.h"
#include "../../core/lv_global.h"

#if LV_USE_DRAW_SW_ASM == LV_DRAW_SW_ASM_HELIUM
    #include "arm2d/lv_draw_sw_helium.h"
#elif LV_USE_DRAW_SW_ASM == LV_DRAW_SW_ASM_CUSTOM
    #include LV_DRAW_SW_ASM_CUSTOM_INCLUDE
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/


static void img_draw_core(lv_draw_task_t * t, const lv_draw_image_dsc_t * draw_dsc,
                          const lv_image_decoder_dsc_t * decoder_dsc, lv_draw_image_sup_t * sup,
                          const lv_area_t * img_coords, const lv_area_t * clipped_img_area);

/**********************
 *  STATIC VARIABLES
 **********************/
#define _draw_info LV_GLOBAL_DEFAULT()->draw_info

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_sw_image(lv_draw_task_t * t, const lv_draw_image_dsc_t * draw_dsc,
                      const lv_area_t * coords)
{
    if(!draw_dsc->tile) {
        lv_draw_image_normal_helper(t, draw_dsc, coords, img_draw_core);
    }
    else {
        lv_draw_image_tiled_helper(t, draw_dsc, coords, img_draw_core);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void img_draw_core(lv_draw_task_t * t, const lv_draw_image_dsc_t * draw_dsc,
                          const lv_image_decoder_dsc_t * decoder_dsc, lv_draw_image_sup_t * sup,
                          const lv_area_t * img_coords, const lv_area_t * clipped_img_area)
{

    const lv_draw_buf_t * decoded = decoder_dsc->decoded;
    const uint8_t * src_buf = decoded->data;
    const lv_image_header_t * header = &decoded->header;
    uint32_t img_stride = decoded->header.stride;
    lv_color_format_t cf = decoded->header.cf;

    lv_draw_sw_blend_dsc_t blend_dsc;
    lv_memzero(&blend_dsc, sizeof(lv_draw_sw_blend_dsc_t));
    blend_dsc.opa = draw_dsc->opa;
    blend_dsc.blend_mode = draw_dsc->blend_mode;
    blend_dsc.src_stride = img_stride;

    if(cf == LV_COLOR_FORMAT_A8) {
        lv_area_t clipped_coords;
        if(!lv_area_intersect(&clipped_coords, img_coords, &t->clip_area)) return;

        blend_dsc.mask_buf = (lv_opa_t *)src_buf;
        blend_dsc.mask_area = img_coords;
        blend_dsc.mask_stride = img_stride;
        blend_dsc.src_buf = NULL;
        blend_dsc.color = draw_dsc->recolor;
        blend_dsc.mask_res = LV_DRAW_SW_MASK_RES_CHANGED;

        blend_dsc.blend_area = img_coords;
        lv_draw_sw_blend(t, &blend_dsc);
    }
    /*The simplest case just copy the pixels into the draw_buf. Blending will convert the colors if needed*/
    else {
        blend_dsc.src_area = img_coords;
        blend_dsc.src_buf = src_buf;
        blend_dsc.blend_area = img_coords;
        blend_dsc.src_color_format = cf;
        lv_draw_sw_blend(t, &blend_dsc);
    }
}

#endif /*LV_USE_DRAW_SW*/
