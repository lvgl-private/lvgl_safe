/**
 * @file lv_draw_sw_fill.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../misc/lv_area_private.h"
#include "lv_draw_sw_mask_private.h"
#include "../lv_draw_private.h"
#include "lv_draw_sw.h"
#if LV_USE_DRAW_SW

#include "blend/lv_draw_sw_blend_private.h"
#include "../../misc/lv_math.h"
#include "../../misc/lv_text_ap.h"
#include "../../core/lv_refr.h"
#include "../../misc/lv_assert.h"
#include "../../stdlib/lv_string.h"
#include "../lv_draw_mask.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_sw_fill(lv_draw_task_t * t, lv_draw_fill_dsc_t * dsc, const lv_area_t * coords)
{
    if(dsc->opa <= LV_OPA_MIN) return;

    lv_area_t bg_coords;
    lv_area_copy(&bg_coords, coords);

    lv_area_t clipped_coords;
    if(!lv_area_intersect(&clipped_coords, &bg_coords, &t->clip_area)) return;

    lv_grad_dir_t grad_dir = dsc->grad.dir;
    lv_color_t bg_color    = grad_dir == LV_GRAD_DIR_NONE ? dsc->color : dsc->grad.stops[0].color;

    lv_draw_sw_blend_dsc_t blend_dsc = {0};
    blend_dsc.color = bg_color;

    /*Most simple case: just a plain rectangle*/
    blend_dsc.blend_area = &bg_coords;
    blend_dsc.opa = dsc->opa;
    lv_draw_sw_blend(t, &blend_dsc);
}

#endif /*LV_USE_DRAW_SW*/
