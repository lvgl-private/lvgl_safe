/**
 * @file lv_obj_pos.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../misc/lv_area_private.h"
#include "lv_obj_event_private.h"
#include "lv_obj_draw_private.h"
#include "lv_obj_style_private.h"
#include "lv_obj_private.h"
#include "../display/lv_display.h"
#include "../display/lv_display_private.h"
#include "lv_refr_private.h"
#include "../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_obj_class)
#define update_layout_mutex LV_GLOBAL_DEFAULT()->layout_update_mutex

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void realign(lv_obj_t * obj);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_obj_set_pos(lv_obj_t * obj, int32_t x, int32_t y)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    obj->x = x;
    obj->y = y;
    realign(obj);
}

void lv_obj_set_x(lv_obj_t * obj, int32_t x)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    obj->x = x;
    realign(obj);
}

void lv_obj_set_y(lv_obj_t * obj, int32_t y)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    obj->y = y;
    realign(obj);
}

void lv_obj_set_size(lv_obj_t * obj, int32_t w, int32_t h)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    obj->w = w;
    obj->h = h;
    realign(obj);
}

void lv_obj_set_width(lv_obj_t * obj, int32_t w)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    obj->w = w;
    realign(obj);
}

void lv_obj_set_height(lv_obj_t * obj, int32_t h)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    obj->w = h;
    realign(obj);
}

void lv_obj_set_align(lv_obj_t * obj, lv_align_t align)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    obj->align = align;
    realign(obj);
}


void lv_obj_get_coords(const lv_obj_t * obj, lv_area_t * coords)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_area_copy(coords, &obj->coords);
}

int32_t lv_obj_get_x(const lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    return obj->x;
}


int32_t lv_obj_get_y(const lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    return obj->y;
}

int32_t lv_obj_get_width(const lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    return lv_area_get_width(&obj->coords);
}

int32_t lv_obj_get_height(const lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    return lv_area_get_height(&obj->coords);
}

void lv_obj_invalidate_area(const lv_obj_t * obj, const lv_area_t * area)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_display_t * disp   = lv_obj_get_display(obj);
    if(!lv_display_is_invalidation_enabled(disp)) return;

    lv_area_t area_tmp;
    lv_area_copy(&area_tmp, area);

    if(!lv_obj_area_is_visible(obj, &area_tmp)) return;
#if LV_DRAW_TRANSFORM_USE_MATRIX
    /**
     * When using the global matrix, the vertex coordinates of clip_area lose precision after transformation,
     * which can be solved by expanding the redrawing area.
     */
    lv_area_increase(&area_tmp, 5, 5);
#else
    if(obj->spec_attr && obj->spec_attr->layer_type == LV_LAYER_TYPE_TRANSFORM) {
        /*Make the area slightly larger to avoid rounding errors.
         *5 is an empirical value*/
        lv_area_increase(&area_tmp, 5, 5);
    }
#endif

    lv_inv_area(lv_obj_get_display(obj),  &area_tmp);
}

void lv_obj_invalidate(const lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_size = lv_obj_get_ext_draw_size(obj);
    lv_area_copy(&obj_coords, &obj->coords);
    obj_coords.x1 -= ext_size;
    obj_coords.y1 -= ext_size;
    obj_coords.x2 += ext_size;
    obj_coords.y2 += ext_size;

    lv_obj_invalidate_area(obj, &obj_coords);
}

bool lv_obj_area_is_visible(const lv_obj_t * obj, lv_area_t * area)
{
    if(lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN)) return false;

    /*Invalidate the object only if it belongs to the current or previous or one of the layers'*/
    lv_obj_t * obj_scr = lv_obj_get_screen(obj);
    lv_display_t * disp   = lv_obj_get_display(obj_scr);
    if(obj_scr != lv_display_get_screen_active(disp) &&
       obj_scr != lv_display_get_screen_prev(disp) &&
       obj_scr != lv_display_get_layer_bottom(disp) &&
       obj_scr != lv_display_get_layer_top(disp) &&
       obj_scr != lv_display_get_layer_sys(disp)) {
        return false;
    }

    /*Truncate the area to the object*/
    lv_area_t obj_coords;
    int32_t ext_size = lv_obj_get_ext_draw_size(obj);
    lv_area_copy(&obj_coords, &obj->coords);
    lv_area_increase(&obj_coords, ext_size, ext_size);

    /*The area is not on the object*/
    if(!lv_area_intersect(area, area, &obj_coords)) return false;

    /*Truncate recursively to the parents*/
    lv_obj_t * parent = lv_obj_get_parent(obj);
    while(parent != NULL) {
        /*If the parent is hidden then the child is hidden and won't be drawn*/
        if(lv_obj_has_flag(parent, LV_OBJ_FLAG_HIDDEN)) return false;

        /*Truncate to the parent and if no common parts break*/
        lv_area_t parent_coords = parent->coords;
        if(lv_obj_has_flag(parent, LV_OBJ_FLAG_OVERFLOW_VISIBLE)) {
            int32_t parent_ext_size = lv_obj_get_ext_draw_size(parent);
            lv_area_increase(&parent_coords, parent_ext_size, parent_ext_size);
        }

        if(!lv_area_intersect(area, area, &parent_coords)) return false;

        parent = lv_obj_get_parent(parent);
    }

    return true;
}

bool lv_obj_is_visible(const lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_area_t obj_coords;
    int32_t ext_size = lv_obj_get_ext_draw_size(obj);
    lv_area_copy(&obj_coords, &obj->coords);
    obj_coords.x1 -= ext_size;
    obj_coords.y1 -= ext_size;
    obj_coords.x2 += ext_size;
    obj_coords.y2 += ext_size;

    return lv_obj_area_is_visible(obj, &obj_coords);
}

void lv_obj_set_ext_click_area(lv_obj_t * obj, int32_t size)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_obj_allocate_spec_attr(obj);
    obj->spec_attr->ext_click_pad = size;
}

void lv_obj_get_click_area(const lv_obj_t * obj, lv_area_t * area)
{
    lv_area_copy(area, &obj->coords);
    if(obj->spec_attr) {
        lv_area_increase(area, obj->spec_attr->ext_click_pad, obj->spec_attr->ext_click_pad);
    }
}

bool lv_obj_hit_test(lv_obj_t * obj, const lv_point_t * point)
{
    if(!lv_obj_has_flag(obj, LV_OBJ_FLAG_CLICKABLE)) return false;

    lv_area_t a;
    lv_obj_get_click_area(obj, &a);
    bool res = lv_area_is_point_on(&a, point, 0);
    if(res == false) return false;

    if(lv_obj_has_flag(obj, LV_OBJ_FLAG_ADV_HITTEST)) {
        lv_hit_test_info_t hit_info;
        hit_info.point = point;
        hit_info.res = true;
        lv_obj_send_event(obj, LV_EVENT_HIT_TEST, &hit_info);
        return hit_info.res;
    }

    return res;
}

void lv_obj_center(lv_obj_t * obj)
{
    lv_obj_set_align(obj, LV_ALIGN_CENTER);
    lv_obj_set_x(obj, 0);
    lv_obj_set_y(obj, 0);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void realign(lv_obj_t * obj)
{
    lv_align_t align = obj->align;
    int32_t pw = lv_obj_get_width(obj->parent);
    int32_t ph = lv_obj_get_height(obj->parent);
    int32_t x = lv_obj_get_x(obj);
    int32_t y = lv_obj_get_y(obj);
    int32_t w = lv_obj_get_width(obj);
    int32_t h = lv_obj_get_height(obj);

    switch(align) {
        case LV_ALIGN_TOP_LEFT:
            break;
        case LV_ALIGN_TOP_MID:
            x += pw / 2 - w / 2;
            break;
        case LV_ALIGN_TOP_RIGHT:
            x += pw - w;
            break;
        case LV_ALIGN_LEFT_MID:
            y += ph / 2 - h / 2;
            break;
        case LV_ALIGN_BOTTOM_LEFT:
            y += ph - h;
            break;
        case LV_ALIGN_BOTTOM_MID:
            x += pw / 2 - w / 2;
            y += ph - h;
            break;
        case LV_ALIGN_BOTTOM_RIGHT:
            x += pw - w;
            y += ph - h;
            break;
        case LV_ALIGN_RIGHT_MID:
            x += pw - w;
            y += ph / 2 - h / 2;
            break;
        case LV_ALIGN_CENTER:
            x += pw / 2 - w / 2;
            y += ph / 2 - h / 2;
            break;
        default:
            break;
    }

    lv_obj_invalidate(obj);

    lv_area_set_width(&obj->coords, obj->w);
    lv_area_set_height(&obj->coords, obj->h);
    lv_area_move(&obj->coords, obj->parent->coords.x1 + x, obj->parent->coords.x1 + y);

    uint32_t i;
    for(i = 0; i < obj->spec_attr->child_cnt; i++) {
        lv_obj_t * child = obj->spec_attr->children[i];
        realign(child);
    }

    lv_obj_invalidate(obj);

}
