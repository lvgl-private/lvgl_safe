/**
 * @file lv_label.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_label_private.h"
#include "../../misc/lv_area_private.h"
#include "../../misc/lv_anim_private.h"
#include "../../draw/lv_draw_label_private.h"
#include "../../core/lv_obj_class_private.h"
#if LV_USE_LABEL != 0
#include "../../core/lv_obj_private.h"
#include "../../misc/lv_assert.h"
#include "../../core/lv_group.h"
#include "../../display/lv_display.h"
#include "../../draw/lv_draw_private.h"
#include "../../misc/lv_color.h"
#include "../../misc/lv_math.h"
#include "../../misc/lv_bidi_private.h"
#include "../../misc/lv_text_ap.h"
#include "../../misc/lv_text_private.h"
#include "../../stdlib/lv_sprintf.h"
#include "../../stdlib/lv_string.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS (&lv_label_class)

#define LV_LABEL_DEF_SCROLL_SPEED   lv_anim_speed_clamped(40, 300, 10000)
#define LV_LABEL_SCROLL_DELAY       300
#define LV_LABEL_DOT_BEGIN_INV 0xFFFFFFFF
#define LV_LABEL_HINT_HEIGHT_LIMIT 1024 /*Enable "hint" to buffer info about labels larger than this. (Speed up drawing)*/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_label_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_label_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_label_event(const lv_obj_class_t * class_p, lv_event_t * e);
static void draw_main(lv_event_t * e);

static void lv_label_refr_text(lv_obj_t * obj);
static void lv_label_revert_dots(lv_obj_t * label);
static void lv_label_set_dots(lv_obj_t * label, uint32_t dot_begin);

static void set_ofs_x_anim(void * obj, int32_t v);
static void set_ofs_y_anim(void * obj, int32_t v);
static size_t get_text_length(const char * text);
static void copy_text_to_label(lv_label_t * label, const char * text);
static lv_text_flag_t get_label_flags(lv_label_t * label);
static void calculate_x_coordinate(int32_t * x, const lv_text_align_t align, const char * txt,
                                   uint32_t length, const lv_font_t * font, int32_t letter_space, lv_area_t * txt_coords, lv_text_flag_t flags);

/**********************
 *  STATIC VARIABLES
 **********************/
#if LV_USE_OBJ_PROPERTY
static const lv_property_ops_t properties[] = {
    {
        .id = LV_PROPERTY_LABEL_TEXT,
        .setter = lv_label_set_text,
        .getter = lv_label_get_text,
    },
    {
        .id = LV_PROPERTY_LABEL_LONG_MODE,
        .setter = lv_label_set_long_mode,
        .getter = lv_label_get_long_mode,
    },
    {
        .id = LV_PROPERTY_LABEL_TEXT_SELECTION_START,
        .setter = lv_label_set_text_selection_start,
        .getter = lv_label_get_text_selection_start,
    },
    {
        .id = LV_PROPERTY_LABEL_TEXT_SELECTION_END,
        .setter = lv_label_set_text_selection_end,
        .getter = lv_label_get_text_selection_end,
    },
};
#endif

const lv_obj_class_t lv_label_class = {
    .constructor_cb = lv_label_constructor,
    .destructor_cb = lv_label_destructor,
    .event_cb = lv_label_event,
    .width_def = 200,
    .height_def = 30,
    .instance_size = sizeof(lv_label_t),
    .base_class = &lv_obj_class,
    .name = "lv_label",
#if LV_USE_OBJ_PROPERTY
    .prop_index_start = LV_PROPERTY_LABEL_START,
    .prop_index_end = LV_PROPERTY_LABEL_END,
    .properties = properties,
    .properties_count = sizeof(properties) / sizeof(properties[0]),

#if LV_USE_OBJ_PROPERTY_NAME
    .property_names = lv_label_property_names,
    .names_count = sizeof(lv_label_property_names) / sizeof(lv_property_name_t),
#endif

#endif
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_label_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/*=====================
 * Setter functions
 *====================*/

void lv_label_set_text(lv_obj_t * obj, const char * text)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_label_t * label = (lv_label_t *)obj;

    /*If text is NULL then just refresh with the current text*/
    if(text == NULL) text = label->text;

    lv_label_revert_dots(obj); /*In case text == label->text*/
    const size_t text_len = get_text_length(text);

    /*If set its own text then reallocate it (maybe its size changed)*/
    if(label->text == text && label->static_txt == 0) {
        label->text = lv_realloc(label->text, text_len);
        LV_ASSERT_MALLOC(label->text);
        if(label->text == NULL) return;

#if LV_USE_ARABIC_PERSIAN_CHARS
        lv_text_ap_proc(label->text, label->text);
#endif

    }
    else {
        /*Free the old text*/
        if(label->text != NULL && label->static_txt == 0) {
            lv_free(label->text);
            label->text = NULL;
        }

        label->text = lv_malloc(text_len);
        LV_ASSERT_MALLOC(label->text);
        if(label->text == NULL) return;

        copy_text_to_label(label, text);

        /*Now the text is dynamically allocated*/
        label->static_txt = 0;
    }

    lv_label_refr_text(obj);
}

void lv_label_set_text_fmt(lv_obj_t * obj, const char * fmt, ...)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    LV_ASSERT_NULL(fmt);

    lv_obj_invalidate(obj);
    lv_label_t * label = (lv_label_t *)obj;

    /*If text is NULL then refresh*/
    if(fmt == NULL) {
        lv_label_refr_text(obj);
        return;
    }

    if(label->text != NULL && label->static_txt == 0) {
        lv_free(label->text);
        label->text = NULL;
    }

    va_list args;
    va_start(args, fmt);
    label->text = lv_text_set_text_vfmt(fmt, args);
    va_end(args);
    label->static_txt = 0; /*Now the text is dynamically allocated*/

    lv_label_refr_text(obj);
}

void lv_label_set_text_static(lv_obj_t * obj, const char * text)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_label_t * label = (lv_label_t *)obj;

    if(label->static_txt == 0 && label->text != NULL) {
        lv_free(label->text);
        label->text = NULL;
    }

    if(text != NULL) {
        label->static_txt = 1;
        label->text       = (char *)text;
    }

    lv_label_refr_text(obj);
}

void lv_label_set_long_mode(lv_obj_t * obj, lv_label_long_mode_t long_mode)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_label_t * label = (lv_label_t *)obj;

    /*Delete the old animation (if exists)*/
    lv_anim_delete(obj, set_ofs_x_anim);
    lv_anim_delete(obj, set_ofs_y_anim);
    lv_point_set(&label->offset, 0, 0);

    if(long_mode == LV_LABEL_LONG_MODE_SCROLL || long_mode == LV_LABEL_LONG_MODE_SCROLL_CIRCULAR ||
       long_mode == LV_LABEL_LONG_MODE_CLIP)
        label->expand = 1;
    else
        label->expand = 0;

    label->long_mode = long_mode;
    lv_label_refr_text(obj);
}

void lv_label_set_text_selection_start(lv_obj_t * obj, uint32_t index)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

#if LV_LABEL_TEXT_SELECTION
    lv_label_t * label = (lv_label_t *)obj;
    label->sel_start   = index;
    lv_obj_invalidate(obj);
#else
    LV_UNUSED(obj);    /*Unused*/
    LV_UNUSED(index);  /*Unused*/
#endif
}

void lv_label_set_text_selection_end(lv_obj_t * obj, uint32_t index)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

#if LV_LABEL_TEXT_SELECTION
    lv_label_t * label = (lv_label_t *)obj;
    label->sel_end     = index;
    lv_obj_invalidate(obj);
#else
    LV_UNUSED(obj);   /*Unused*/
    LV_UNUSED(index); /*Unused*/
#endif
}

void lv_label_set_recolor(lv_obj_t * obj, bool en)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_label_t * label = (lv_label_t *)obj;
    if(label->recolor == en) return;

    label->recolor = en == false ? 0 : 1;

    /*Refresh the text because the potential color codes in text needs to be hidden or revealed*/
    lv_label_refr_text(obj);
}

/*=====================
 * Getter functions
 *====================*/

char * lv_label_get_text(const lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_label_t * label = (lv_label_t *)obj;
    return label->text;
}

lv_label_long_mode_t lv_label_get_long_mode(const lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_label_t * label = (lv_label_t *)obj;
    return label->long_mode;
}

bool lv_label_get_recolor(const lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    lv_label_t * label = (lv_label_t *)obj;
    return label->recolor == 0 ? false : true;
}

/*=====================
 * Other functions
 *====================*/

void lv_label_ins_text(lv_obj_t * obj, uint32_t pos, const char * txt)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    LV_ASSERT_NULL(txt);

    lv_label_t * label = (lv_label_t *)obj;

    /*Cannot append to static text*/
    if(label->static_txt != 0) return;

    lv_obj_invalidate(obj);

    /*Allocate space for the new text*/
    size_t old_len = lv_strlen(label->text);
    size_t ins_len = lv_strlen(txt);
    size_t new_len = ins_len + old_len;
    label->text        = lv_realloc(label->text, new_len + 1);
    LV_ASSERT_MALLOC(label->text);
    if(label->text == NULL) return;

    if(pos == LV_LABEL_POS_LAST) {
        pos = lv_text_get_encoded_length(label->text);
    }

    lv_text_ins(label->text, pos, txt);
    lv_label_set_text(obj, NULL);
}

void lv_label_cut_text(lv_obj_t * obj, uint32_t pos, uint32_t cnt)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_label_t * label = (lv_label_t *)obj;

    /*Cannot append to static text*/
    if(label->static_txt) return;

    lv_obj_invalidate(obj);

    char * label_txt = lv_label_get_text(obj);
    /*Delete the characters*/
    lv_text_cut(label_txt, pos, cnt);

    /*Refresh the label*/
    lv_label_refr_text(obj);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_label_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    LV_TRACE_OBJ_CREATE("begin");

    lv_label_t * label = (lv_label_t *)obj;

    label->text       = NULL;
    label->recolor    = 0;
    label->static_txt = 0;
    label->dot_begin  = LV_LABEL_DOT_BEGIN_INV;
    label->long_mode  = LV_LABEL_LONG_MODE_WRAP;
    lv_point_set(&label->offset, 0, 0);

#if LV_LABEL_LONG_TXT_HINT
    label->hint.line_start = -1;
    label->hint.coord_y    = 0;
    label->hint.y          = 0;
#endif

#if LV_LABEL_TEXT_SELECTION
    label->sel_start = LV_DRAW_LABEL_NO_TXT_SEL;
    label->sel_end   = LV_DRAW_LABEL_NO_TXT_SEL;
#endif

    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
    lv_label_set_long_mode(obj, LV_LABEL_LONG_MODE_WRAP);
    lv_label_set_text(obj, LV_LABEL_DEFAULT_TEXT);

    LV_TRACE_OBJ_CREATE("finished");
}

static void lv_label_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    lv_label_t * label = (lv_label_t *)obj;

    if(!label->static_txt) lv_free(label->text);
    label->text = NULL;
}

static void lv_label_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    /*Call the ancestor's event handler*/
    const lv_result_t res = lv_obj_event_base(MY_CLASS, e);
    if(res != LV_RESULT_OK) return;

    const lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);

    if((code == LV_EVENT_STYLE_CHANGED) || (code == LV_EVENT_SIZE_CHANGED)) {
        lv_label_refr_text(obj);
    }
    else if(code == LV_EVENT_REFR_EXT_DRAW_SIZE) {
        /* Italic or other non-typical letters can be drawn of out of the object.
         * It happens if box_w + ofs_x > adw_w in the glyph.
         * To avoid this add some extra draw area.
         * font_h / 4 is an empirical value. */
        const lv_font_t * font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
        const int32_t font_h = lv_font_get_line_height(font);
        lv_event_set_ext_draw_size(e, font_h / 4);
    }
    else if(code == LV_EVENT_DRAW_MAIN) {
        draw_main(e);
    }
}

static void draw_main(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_label_t * label = (lv_label_t *)obj;
    lv_layer_t * layer = lv_event_get_layer(e);

    lv_area_t txt_coords = obj->coords;

    lv_text_flag_t flag = get_label_flags(label);

    lv_draw_label_dsc_t label_draw_dsc;
    lv_draw_label_dsc_init(&label_draw_dsc);
    label_draw_dsc.text = label->text;
    label_draw_dsc.text_static = label->static_txt;
    label_draw_dsc.ofs_x = label->offset.x;
    label_draw_dsc.ofs_y = label->offset.y;
    label_draw_dsc.text_size = label->text_size;
#if LV_LABEL_LONG_TXT_HINT
    if(label->long_mode != LV_LABEL_LONG_MODE_SCROLL_CIRCULAR &&
       lv_area_get_height(&txt_coords) >= LV_LABEL_HINT_HEIGHT_LIMIT) {
        label_draw_dsc.hint = &label->hint;
    }
#endif

    label_draw_dsc.flag = flag;
    label_draw_dsc.base.layer = layer;
    lv_obj_init_draw_label_dsc(obj, LV_PART_MAIN, &label_draw_dsc);
    lv_bidi_calculate_align(&label_draw_dsc.align, &label_draw_dsc.bidi_dir, label->text);


    /* get the style attributes of a letter outline */
    label_draw_dsc.outline_stroke_color = lv_obj_get_style_text_outline_stroke_color(obj, LV_PART_MAIN);
    label_draw_dsc.outline_stroke_opa = lv_obj_get_style_text_outline_stroke_opa(obj, LV_PART_MAIN);
    label_draw_dsc.outline_stroke_width = lv_obj_get_style_text_outline_stroke_width(obj, LV_PART_MAIN);


    /* In SCROLL and SCROLL_CIRCULAR mode the CENTER and RIGHT are pointless, so remove them.
     * (In addition, they will create misalignment in this situation)*/
    if((label->long_mode == LV_LABEL_LONG_MODE_SCROLL || label->long_mode == LV_LABEL_LONG_MODE_SCROLL_CIRCULAR) &&
       (label_draw_dsc.align == LV_TEXT_ALIGN_CENTER || label_draw_dsc.align == LV_TEXT_ALIGN_RIGHT)) {
        lv_point_t size = label->text_size;
        if(size.x > lv_area_get_width(&txt_coords)) {
#if LV_USE_BIDI
            const lv_base_dir_t base_dir = lv_obj_get_style_base_dir(obj, LV_PART_MAIN);
            label_draw_dsc.align = base_dir == LV_BASE_DIR_RTL ? LV_TEXT_ALIGN_RIGHT : LV_TEXT_ALIGN_LEFT;
#else
            label_draw_dsc.align = LV_TEXT_ALIGN_LEFT;
#endif
        }
    }

    lv_area_t txt_clip;
    bool is_common = lv_area_intersect(&txt_clip, &txt_coords, &layer->_clip_area);
    if(!is_common) {
        return;
    }

    if(label->long_mode == LV_LABEL_LONG_MODE_WRAP) {
        int32_t s = lv_obj_get_scroll_top(obj);
        lv_area_move(&txt_coords, 0, -s);
        txt_coords.y2 = obj->coords.y2;
    }
    if(label->long_mode == LV_LABEL_LONG_MODE_SCROLL ||
       label->long_mode == LV_LABEL_LONG_MODE_SCROLL_CIRCULAR ||
       label->long_mode == LV_LABEL_LONG_MODE_CLIP) {
        const lv_area_t clip_area_ori = layer->_clip_area;
        layer->_clip_area = txt_clip;
        lv_draw_label(layer, &label_draw_dsc, &txt_coords);
        layer->_clip_area = clip_area_ori;
    }
    else {
        lv_draw_label(layer, &label_draw_dsc, &txt_coords);
    }

    lv_area_t clip_area_ori = layer->_clip_area;
    layer->_clip_area = txt_clip;

    if(label->long_mode == LV_LABEL_LONG_MODE_SCROLL_CIRCULAR) {
        lv_point_t size = label->text_size;

        /*Draw the text again on label to the original to make a circular effect */
        if(size.x > lv_area_get_width(&txt_coords)) {
            label_draw_dsc.ofs_x = label->offset.x + size.x +
                                   lv_font_get_glyph_width(label_draw_dsc.font, ' ', ' ') * LV_LABEL_WAIT_CHAR_COUNT;
            label_draw_dsc.ofs_y = label->offset.y;

            lv_draw_label(layer, &label_draw_dsc, &txt_coords);
        }

        /*Draw the text again below the original to make a circular effect */
        if(size.y > lv_area_get_height(&txt_coords)) {
            label_draw_dsc.ofs_x = label->offset.x;
            label_draw_dsc.ofs_y = label->offset.y + size.y + lv_font_get_line_height(label_draw_dsc.font);

            lv_draw_label(layer, &label_draw_dsc, &txt_coords);
        }
    }

    layer->_clip_area = clip_area_ori;
}

static void overwrite_anim_property(lv_anim_t * dest, const lv_anim_t * src, lv_label_long_mode_t mode)
{
    switch(mode) {
        case LV_LABEL_LONG_MODE_SCROLL:
            /* If the dest animation is already running, overwrite is not allowed */
            if(dest->act_time <= 0)
                dest->act_time = src->act_time;
            dest->repeat_cnt = src->repeat_cnt;
            dest->repeat_delay = src->repeat_delay;
            dest->completed_cb = src->completed_cb;
            dest->reverse_delay = src->reverse_delay;
            break;
        case LV_LABEL_LONG_MODE_SCROLL_CIRCULAR:
            /* If the dest animation is already running, overwrite is not allowed */
            if(dest->act_time <= 0)
                dest->act_time = src->act_time;
            dest->repeat_cnt = src->repeat_cnt;
            dest->repeat_delay = src->repeat_delay;
            dest->completed_cb = src->completed_cb;
            break;
        default:
            break;
    }
}

/**
 * Refresh the label with its text stored in its extended data
 * @param label pointer to a label object
 */
static void lv_label_refr_text(lv_obj_t * obj)
{
    lv_label_t * label = (lv_label_t *)obj;
    if(label->text == NULL) return;
#if LV_LABEL_LONG_TXT_HINT
    label->hint.line_start = -1; /*The hint is invalid if the text changes*/
#endif
    label->invalid_size_cache = true;

    lv_area_t txt_coords = obj->coords;
    int32_t max_w         = lv_area_get_width(&txt_coords);
    const lv_font_t * font   = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
    int32_t line_space = lv_obj_get_style_text_line_space(obj, LV_PART_MAIN);
    int32_t letter_space = lv_obj_get_style_text_letter_space(obj, LV_PART_MAIN);

    /*Calc. the height and longest line*/
    lv_point_t size;
    lv_text_flag_t flag = get_label_flags(label);

    lv_label_revert_dots(obj);
    lv_text_get_size(&size, label->text, font, letter_space, line_space, max_w, flag);
    label->text_size = size;


    lv_obj_invalidate(obj);
}

static void lv_label_revert_dots(lv_obj_t * obj)
{
    lv_label_t * label = (lv_label_t *)obj;
    if(label->dot_begin != LV_LABEL_DOT_BEGIN_INV) {
        for(int i = 0; i < LV_LABEL_DOT_NUM + 1 && label->dot[i]; i++) {
            label->text[label->dot_begin + i] = label->dot[i];
        }
        label->dot_begin = LV_LABEL_DOT_BEGIN_INV;
    }
}

static void lv_label_set_dots(lv_obj_t * obj, uint32_t dot_begin)
{
    lv_label_t * label = (lv_label_t *)obj;
    LV_ASSERT_MSG(label->dot_begin == LV_LABEL_DOT_BEGIN_INV, "Label dots already set");
    if(dot_begin != LV_LABEL_DOT_BEGIN_INV) {
        /*Save characters*/
        lv_strncpy(label->dot, &label->text[dot_begin], LV_LABEL_DOT_NUM + 1);
        label->dot_begin = dot_begin;

        /*Overwrite up to LV_LABEL_DOT_NUM + 1 characters with dots and null terminator*/
        int i = 0;
        for(; i < LV_LABEL_DOT_NUM && label->text[dot_begin + i]; i++) {
            label->text[dot_begin + i] = '.';
        }
        label->text[dot_begin + i] = '\0';
    }
}

static void set_ofs_x_anim(void * obj, int32_t v)
{
    lv_label_t * label = (lv_label_t *)obj;
    label->offset.x    = v;
    lv_obj_invalidate(obj);
}

static void set_ofs_y_anim(void * obj, int32_t v)
{
    lv_label_t * label = (lv_label_t *)obj;
    label->offset.y    = v;
    lv_obj_invalidate(obj);
}

static size_t get_text_length(const char * text)
{
    size_t len = 0;
#if LV_USE_ARABIC_PERSIAN_CHARS
    len = lv_text_ap_calc_bytes_count(text);
#else
    len = lv_strlen(text) + 1;
#endif

    return len;
}

static void copy_text_to_label(lv_label_t * label, const char * text)
{
#if LV_USE_ARABIC_PERSIAN_CHARS
    lv_text_ap_proc(text, label->text);
#else
    lv_strcpy(label->text, text);
#endif
}

static lv_text_flag_t get_label_flags(lv_label_t * label)
{
    lv_text_flag_t flag = LV_TEXT_FLAG_NONE;

    if(label->recolor) flag |= LV_TEXT_FLAG_RECOLOR;
    if(label->expand) flag |= LV_TEXT_FLAG_EXPAND;

    return flag;
}

#endif
