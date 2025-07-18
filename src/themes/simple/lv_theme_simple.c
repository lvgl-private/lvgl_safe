/**
 * @file lv_theme_simple.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../lv_theme_private.h"
#include "../../../lvgl.h" /*To see all the widgets*/

#if LV_USE_THEME_SIMPLE

#include "lv_theme_simple.h"
#include "../../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/

struct _my_theme_t;
typedef struct _my_theme_t my_theme_t;

#define theme_def (*(my_theme_t **)(&LV_GLOBAL_DEFAULT()->theme_simple))

#define COLOR_SCR     lv_palette_lighten(LV_PALETTE_GREY, 4)
#define COLOR_WHITE   lv_color_white()
#define COLOR_LIGHT   lv_palette_lighten(LV_PALETTE_GREY, 2)
#define COLOR_DARK    lv_palette_main(LV_PALETTE_GREY)
#define COLOR_DIM     lv_palette_darken(LV_PALETTE_GREY, 2)
#define SCROLLBAR_WIDTH     2

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    lv_style_t scr;
    lv_style_t transp;
    lv_style_t white;
    lv_style_t light;
    lv_style_t dark;
    lv_style_t dim;
    lv_style_t scrollbar;
} my_theme_styles_t;

struct _my_theme_t {
    lv_theme_t base;
    my_theme_styles_t styles;
    bool inited;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void style_init_reset(lv_style_t * style);
static void theme_apply(lv_theme_t * th, lv_obj_t * obj);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void style_init(my_theme_t * theme)
{
    style_init_reset(&theme->styles.scrollbar);
    lv_style_set_bg_opa(&theme->styles.scrollbar, LV_OPA_COVER);
    lv_style_set_bg_color(&theme->styles.scrollbar, COLOR_DARK);
    lv_style_set_width(&theme->styles.scrollbar, SCROLLBAR_WIDTH);

    style_init_reset(&theme->styles.scr);
    lv_style_set_bg_opa(&theme->styles.scr, LV_OPA_COVER);
    lv_style_set_bg_color(&theme->styles.scr, COLOR_SCR);
    lv_style_set_text_color(&theme->styles.scr, COLOR_DIM);

    style_init_reset(&theme->styles.transp);
    lv_style_set_bg_opa(&theme->styles.transp, LV_OPA_TRANSP);

    style_init_reset(&theme->styles.white);
    lv_style_set_bg_opa(&theme->styles.white, LV_OPA_COVER);
    lv_style_set_bg_color(&theme->styles.white, COLOR_WHITE);
    lv_style_set_line_width(&theme->styles.white, 1);
    lv_style_set_line_color(&theme->styles.white, COLOR_WHITE);
    lv_style_set_arc_width(&theme->styles.white, 2);
    lv_style_set_arc_color(&theme->styles.white, COLOR_WHITE);

    style_init_reset(&theme->styles.light);
    lv_style_set_bg_opa(&theme->styles.light, LV_OPA_COVER);
    lv_style_set_bg_color(&theme->styles.light, COLOR_LIGHT);
    lv_style_set_line_width(&theme->styles.light, 1);
    lv_style_set_line_color(&theme->styles.light, COLOR_LIGHT);
    lv_style_set_arc_width(&theme->styles.light, 2);
    lv_style_set_arc_color(&theme->styles.light, COLOR_LIGHT);

    style_init_reset(&theme->styles.dark);
    lv_style_set_bg_opa(&theme->styles.dark, LV_OPA_COVER);
    lv_style_set_bg_color(&theme->styles.dark, COLOR_DARK);
    lv_style_set_line_width(&theme->styles.dark, 1);
    lv_style_set_line_color(&theme->styles.dark, COLOR_DARK);
    lv_style_set_arc_width(&theme->styles.dark, 2);
    lv_style_set_arc_color(&theme->styles.dark, COLOR_DARK);

    style_init_reset(&theme->styles.dim);
    lv_style_set_bg_opa(&theme->styles.dim, LV_OPA_COVER);
    lv_style_set_bg_color(&theme->styles.dim, COLOR_DIM);
    lv_style_set_line_width(&theme->styles.dim, 1);
    lv_style_set_line_color(&theme->styles.dim, COLOR_DIM);
    lv_style_set_arc_width(&theme->styles.dim, 2);
    lv_style_set_arc_color(&theme->styles.dim, COLOR_DIM);

}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_theme_t * lv_theme_simple_init(lv_display_t * disp)
{
    /*This trick is required only to avoid the garbage collection of
     *styles' data if LVGL is used in a binding (e.g. MicroPython)
     *In a general case styles could be in a simple `static lv_style_t my_style...` variables*/
    if(!lv_theme_simple_is_inited()) {
        theme_def = lv_malloc_zeroed(sizeof(my_theme_t));
    }

    my_theme_t * theme = theme_def;

    theme->base.disp = disp;
    theme->base.font_small = LV_FONT_DEFAULT;
    theme->base.font_normal = LV_FONT_DEFAULT;
    theme->base.font_large = LV_FONT_DEFAULT;
    theme->base.apply_cb = theme_apply;

    style_init(theme);

    if(disp == NULL || lv_display_get_theme(disp) == (lv_theme_t *)theme) {
        lv_obj_report_style_change(NULL);
    }

    theme->inited = true;

    return (lv_theme_t *)theme_def;
}

bool lv_theme_simple_is_inited(void)
{
    my_theme_t * theme = theme_def;
    if(theme == NULL) return false;
    return theme->inited;
}

lv_theme_t * lv_theme_simple_get(void)
{
    if(!lv_theme_simple_is_inited()) {
        return NULL;
    }

    return (lv_theme_t *)theme_def;
}

void lv_theme_simple_deinit(void)
{
    my_theme_t * theme = theme_def;
    if(theme) {
        if(theme->inited) {
            lv_style_t * theme_styles = (lv_style_t *)(&(theme->styles));
            uint32_t i;
            for(i = 0; i < sizeof(my_theme_styles_t) / sizeof(lv_style_t); i++) {
                lv_style_reset(theme_styles + i);
            }
        }
        lv_free(theme_def);
        theme_def = NULL;
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void theme_apply(lv_theme_t * th, lv_obj_t * obj)
{
    LV_UNUSED(th);

    my_theme_t * theme = theme_def;
    lv_obj_t * parent = lv_obj_get_parent(obj);

    if(parent == NULL) {
        lv_obj_add_style(obj, &theme->styles.scr, 0);
        lv_obj_add_style(obj, &theme->styles.scrollbar, LV_PART_SCROLLBAR);
        return;
    }


#if LV_USE_BAR
    else if(lv_obj_check_type(obj, &lv_bar_class)) {
        lv_obj_add_style(obj, &theme->styles.light, 0);
        lv_obj_add_style(obj, &theme->styles.dark, LV_PART_INDICATOR);
    }
#endif

#if LV_USE_SLIDER
    else if(lv_obj_check_type(obj, &lv_slider_class)) {
        lv_obj_add_style(obj, &theme->styles.light, 0);
        lv_obj_add_style(obj, &theme->styles.dark, LV_PART_INDICATOR);
        lv_obj_add_style(obj, &theme->styles.dim, LV_PART_KNOB);
    }
#endif


#if LV_USE_CHECKBOX
    else if(lv_obj_check_type(obj, &lv_checkbox_class)) {
        lv_obj_add_style(obj, &theme->styles.light, LV_PART_INDICATOR);
        lv_obj_add_style(obj, &theme->styles.dark, LV_PART_INDICATOR | LV_STATE_CHECKED);
    }
#endif

#if LV_USE_SWITCH
    else if(lv_obj_check_type(obj, &lv_switch_class)) {
        lv_obj_add_style(obj, &theme->styles.light, 0);
        lv_obj_add_style(obj, &theme->styles.dim, LV_PART_KNOB);
    }
#endif

#if LV_USE_LED
    else if(lv_obj_check_type(obj, &lv_led_class)) {
        lv_obj_add_style(obj, &theme->styles.light, 0);
    }
#endif
}

static void style_init_reset(lv_style_t * style)
{
    if(lv_theme_simple_is_inited()) {
        lv_style_reset(style);
    }
    else {
        lv_style_init(style);
    }
}

#endif
