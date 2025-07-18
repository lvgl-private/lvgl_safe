
/*
 **********************************************************************
 *                            DO NOT EDIT
 * This file is automatically generated by "style_api_gen.py"
 **********************************************************************
 */


#ifndef LV_STYLE_GEN_H
#define LV_STYLE_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

void lv_style_set_width(lv_style_t * style, int32_t value);
void lv_style_set_min_width(lv_style_t * style, int32_t value);
void lv_style_set_max_width(lv_style_t * style, int32_t value);
void lv_style_set_height(lv_style_t * style, int32_t value);
void lv_style_set_min_height(lv_style_t * style, int32_t value);
void lv_style_set_max_height(lv_style_t * style, int32_t value);
void lv_style_set_length(lv_style_t * style, int32_t value);
void lv_style_set_x(lv_style_t * style, int32_t value);
void lv_style_set_y(lv_style_t * style, int32_t value);
void lv_style_set_align(lv_style_t * style, lv_align_t value);
void lv_style_set_transform_width(lv_style_t * style, int32_t value);
void lv_style_set_transform_height(lv_style_t * style, int32_t value);
void lv_style_set_translate_x(lv_style_t * style, int32_t value);
void lv_style_set_translate_y(lv_style_t * style, int32_t value);
void lv_style_set_translate_radial(lv_style_t * style, int32_t value);
void lv_style_set_transform_scale_x(lv_style_t * style, int32_t value);
void lv_style_set_transform_scale_y(lv_style_t * style, int32_t value);
void lv_style_set_transform_rotation(lv_style_t * style, int32_t value);
void lv_style_set_transform_pivot_x(lv_style_t * style, int32_t value);
void lv_style_set_transform_pivot_y(lv_style_t * style, int32_t value);
void lv_style_set_transform_skew_x(lv_style_t * style, int32_t value);
void lv_style_set_transform_skew_y(lv_style_t * style, int32_t value);
void lv_style_set_pad_top(lv_style_t * style, int32_t value);
void lv_style_set_pad_bottom(lv_style_t * style, int32_t value);
void lv_style_set_pad_left(lv_style_t * style, int32_t value);
void lv_style_set_pad_right(lv_style_t * style, int32_t value);
void lv_style_set_pad_row(lv_style_t * style, int32_t value);
void lv_style_set_pad_column(lv_style_t * style, int32_t value);
void lv_style_set_pad_radial(lv_style_t * style, int32_t value);
void lv_style_set_margin_top(lv_style_t * style, int32_t value);
void lv_style_set_margin_bottom(lv_style_t * style, int32_t value);
void lv_style_set_margin_left(lv_style_t * style, int32_t value);
void lv_style_set_margin_right(lv_style_t * style, int32_t value);
void lv_style_set_bg_color(lv_style_t * style, lv_color_t value);
void lv_style_set_bg_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_bg_grad_color(lv_style_t * style, lv_color_t value);
void lv_style_set_bg_grad_dir(lv_style_t * style, lv_grad_dir_t value);
void lv_style_set_bg_main_stop(lv_style_t * style, int32_t value);
void lv_style_set_bg_grad_stop(lv_style_t * style, int32_t value);
void lv_style_set_bg_main_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_bg_grad_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_bg_grad(lv_style_t * style, const lv_grad_dsc_t * value);
void lv_style_set_bg_image_src(lv_style_t * style, const void * value);
void lv_style_set_bg_image_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_bg_image_recolor(lv_style_t * style, lv_color_t value);
void lv_style_set_bg_image_recolor_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_bg_image_tiled(lv_style_t * style, bool value);
void lv_style_set_border_color(lv_style_t * style, lv_color_t value);
void lv_style_set_border_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_border_width(lv_style_t * style, int32_t value);
void lv_style_set_border_side(lv_style_t * style, lv_border_side_t value);
void lv_style_set_border_post(lv_style_t * style, bool value);
void lv_style_set_outline_width(lv_style_t * style, int32_t value);
void lv_style_set_outline_color(lv_style_t * style, lv_color_t value);
void lv_style_set_outline_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_outline_pad(lv_style_t * style, int32_t value);
void lv_style_set_shadow_width(lv_style_t * style, int32_t value);
void lv_style_set_shadow_offset_x(lv_style_t * style, int32_t value);
void lv_style_set_shadow_offset_y(lv_style_t * style, int32_t value);
void lv_style_set_shadow_spread(lv_style_t * style, int32_t value);
void lv_style_set_shadow_color(lv_style_t * style, lv_color_t value);
void lv_style_set_shadow_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_image_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_image_recolor(lv_style_t * style, lv_color_t value);
void lv_style_set_image_recolor_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_line_width(lv_style_t * style, int32_t value);
void lv_style_set_line_dash_width(lv_style_t * style, int32_t value);
void lv_style_set_line_dash_gap(lv_style_t * style, int32_t value);
void lv_style_set_line_rounded(lv_style_t * style, bool value);
void lv_style_set_line_color(lv_style_t * style, lv_color_t value);
void lv_style_set_line_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_arc_width(lv_style_t * style, int32_t value);
void lv_style_set_arc_rounded(lv_style_t * style, bool value);
void lv_style_set_arc_color(lv_style_t * style, lv_color_t value);
void lv_style_set_arc_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_arc_image_src(lv_style_t * style, const void * value);
void lv_style_set_text_color(lv_style_t * style, lv_color_t value);
void lv_style_set_text_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_text_font(lv_style_t * style, const lv_font_t * value);
void lv_style_set_text_letter_space(lv_style_t * style, int32_t value);
void lv_style_set_text_line_space(lv_style_t * style, int32_t value);
void lv_style_set_text_decor(lv_style_t * style, lv_text_decor_t value);
void lv_style_set_text_align(lv_style_t * style, lv_text_align_t value);
void lv_style_set_text_outline_stroke_color(lv_style_t * style, lv_color_t value);
void lv_style_set_text_outline_stroke_width(lv_style_t * style, int32_t value);
void lv_style_set_text_outline_stroke_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_radius(lv_style_t * style, int32_t value);
void lv_style_set_radial_offset(lv_style_t * style, int32_t value);
void lv_style_set_clip_corner(lv_style_t * style, bool value);
void lv_style_set_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_opa_layered(lv_style_t * style, lv_opa_t value);
void lv_style_set_color_filter_dsc(lv_style_t * style, const lv_color_filter_dsc_t * value);
void lv_style_set_color_filter_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_recolor(lv_style_t * style, lv_color_t value);
void lv_style_set_recolor_opa(lv_style_t * style, lv_opa_t value);
void lv_style_set_anim(lv_style_t * style, const lv_anim_t * value);
void lv_style_set_anim_duration(lv_style_t * style, uint32_t value);
void lv_style_set_transition(lv_style_t * style, const lv_style_transition_dsc_t * value);
void lv_style_set_blend_mode(lv_style_t * style, lv_blend_mode_t value);
void lv_style_set_layout(lv_style_t * style, uint16_t value);
void lv_style_set_base_dir(lv_style_t * style, lv_base_dir_t value);
void lv_style_set_bitmap_mask_src(lv_style_t * style, const void * value);
void lv_style_set_rotary_sensitivity(lv_style_t * style, uint32_t value);

#define LV_STYLE_CONST_WIDTH(val) \
    { \
        .prop = LV_STYLE_WIDTH, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_MIN_WIDTH(val) \
    { \
        .prop = LV_STYLE_MIN_WIDTH, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_MAX_WIDTH(val) \
    { \
        .prop = LV_STYLE_MAX_WIDTH, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_HEIGHT(val) \
    { \
        .prop = LV_STYLE_HEIGHT, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_MIN_HEIGHT(val) \
    { \
        .prop = LV_STYLE_MIN_HEIGHT, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_MAX_HEIGHT(val) \
    { \
        .prop = LV_STYLE_MAX_HEIGHT, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_LENGTH(val) \
    { \
        .prop = LV_STYLE_LENGTH, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_X(val) \
    { \
        .prop = LV_STYLE_X, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_Y(val) \
    { \
        .prop = LV_STYLE_Y, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_ALIGN(val) \
    { \
        .prop = LV_STYLE_ALIGN, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSFORM_WIDTH(val) \
    { \
        .prop = LV_STYLE_TRANSFORM_WIDTH, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSFORM_HEIGHT(val) \
    { \
        .prop = LV_STYLE_TRANSFORM_HEIGHT, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSLATE_X(val) \
    { \
        .prop = LV_STYLE_TRANSLATE_X, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSLATE_Y(val) \
    { \
        .prop = LV_STYLE_TRANSLATE_Y, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSLATE_RADIAL(val) \
    { \
        .prop = LV_STYLE_TRANSLATE_RADIAL, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSFORM_SCALE_X(val) \
    { \
        .prop = LV_STYLE_TRANSFORM_SCALE_X, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSFORM_SCALE_Y(val) \
    { \
        .prop = LV_STYLE_TRANSFORM_SCALE_Y, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSFORM_ROTATION(val) \
    { \
        .prop = LV_STYLE_TRANSFORM_ROTATION, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSFORM_PIVOT_X(val) \
    { \
        .prop = LV_STYLE_TRANSFORM_PIVOT_X, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSFORM_PIVOT_Y(val) \
    { \
        .prop = LV_STYLE_TRANSFORM_PIVOT_Y, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSFORM_SKEW_X(val) \
    { \
        .prop = LV_STYLE_TRANSFORM_SKEW_X, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSFORM_SKEW_Y(val) \
    { \
        .prop = LV_STYLE_TRANSFORM_SKEW_Y, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_PAD_TOP(val) \
    { \
        .prop = LV_STYLE_PAD_TOP, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_PAD_BOTTOM(val) \
    { \
        .prop = LV_STYLE_PAD_BOTTOM, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_PAD_LEFT(val) \
    { \
        .prop = LV_STYLE_PAD_LEFT, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_PAD_RIGHT(val) \
    { \
        .prop = LV_STYLE_PAD_RIGHT, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_PAD_ROW(val) \
    { \
        .prop = LV_STYLE_PAD_ROW, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_PAD_COLUMN(val) \
    { \
        .prop = LV_STYLE_PAD_COLUMN, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_PAD_RADIAL(val) \
    { \
        .prop = LV_STYLE_PAD_RADIAL, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_MARGIN_TOP(val) \
    { \
        .prop = LV_STYLE_MARGIN_TOP, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_MARGIN_BOTTOM(val) \
    { \
        .prop = LV_STYLE_MARGIN_BOTTOM, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_MARGIN_LEFT(val) \
    { \
        .prop = LV_STYLE_MARGIN_LEFT, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_MARGIN_RIGHT(val) \
    { \
        .prop = LV_STYLE_MARGIN_RIGHT, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BG_COLOR(val) \
    { \
        .prop = LV_STYLE_BG_COLOR, .value = { .color = val } \
    }

#define LV_STYLE_CONST_BG_OPA(val) \
    { \
        .prop = LV_STYLE_BG_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BG_GRAD_COLOR(val) \
    { \
        .prop = LV_STYLE_BG_GRAD_COLOR, .value = { .color = val } \
    }

#define LV_STYLE_CONST_BG_GRAD_DIR(val) \
    { \
        .prop = LV_STYLE_BG_GRAD_DIR, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BG_MAIN_STOP(val) \
    { \
        .prop = LV_STYLE_BG_MAIN_STOP, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BG_GRAD_STOP(val) \
    { \
        .prop = LV_STYLE_BG_GRAD_STOP, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BG_MAIN_OPA(val) \
    { \
        .prop = LV_STYLE_BG_MAIN_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BG_GRAD_OPA(val) \
    { \
        .prop = LV_STYLE_BG_GRAD_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BG_GRAD(val) \
    { \
        .prop = LV_STYLE_BG_GRAD, .value = { .ptr = val } \
    }

#define LV_STYLE_CONST_BG_IMAGE_SRC(val) \
    { \
        .prop = LV_STYLE_BG_IMAGE_SRC, .value = { .ptr = val } \
    }

#define LV_STYLE_CONST_BG_IMAGE_OPA(val) \
    { \
        .prop = LV_STYLE_BG_IMAGE_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BG_IMAGE_RECOLOR(val) \
    { \
        .prop = LV_STYLE_BG_IMAGE_RECOLOR, .value = { .color = val } \
    }

#define LV_STYLE_CONST_BG_IMAGE_RECOLOR_OPA(val) \
    { \
        .prop = LV_STYLE_BG_IMAGE_RECOLOR_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BG_IMAGE_TILED(val) \
    { \
        .prop = LV_STYLE_BG_IMAGE_TILED, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BORDER_COLOR(val) \
    { \
        .prop = LV_STYLE_BORDER_COLOR, .value = { .color = val } \
    }

#define LV_STYLE_CONST_BORDER_OPA(val) \
    { \
        .prop = LV_STYLE_BORDER_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BORDER_WIDTH(val) \
    { \
        .prop = LV_STYLE_BORDER_WIDTH, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BORDER_SIDE(val) \
    { \
        .prop = LV_STYLE_BORDER_SIDE, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BORDER_POST(val) \
    { \
        .prop = LV_STYLE_BORDER_POST, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_OUTLINE_WIDTH(val) \
    { \
        .prop = LV_STYLE_OUTLINE_WIDTH, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_OUTLINE_COLOR(val) \
    { \
        .prop = LV_STYLE_OUTLINE_COLOR, .value = { .color = val } \
    }

#define LV_STYLE_CONST_OUTLINE_OPA(val) \
    { \
        .prop = LV_STYLE_OUTLINE_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_OUTLINE_PAD(val) \
    { \
        .prop = LV_STYLE_OUTLINE_PAD, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_SHADOW_WIDTH(val) \
    { \
        .prop = LV_STYLE_SHADOW_WIDTH, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_SHADOW_OFFSET_X(val) \
    { \
        .prop = LV_STYLE_SHADOW_OFFSET_X, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_SHADOW_OFFSET_Y(val) \
    { \
        .prop = LV_STYLE_SHADOW_OFFSET_Y, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_SHADOW_SPREAD(val) \
    { \
        .prop = LV_STYLE_SHADOW_SPREAD, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_SHADOW_COLOR(val) \
    { \
        .prop = LV_STYLE_SHADOW_COLOR, .value = { .color = val } \
    }

#define LV_STYLE_CONST_SHADOW_OPA(val) \
    { \
        .prop = LV_STYLE_SHADOW_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_IMAGE_OPA(val) \
    { \
        .prop = LV_STYLE_IMAGE_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_IMAGE_RECOLOR(val) \
    { \
        .prop = LV_STYLE_IMAGE_RECOLOR, .value = { .color = val } \
    }

#define LV_STYLE_CONST_IMAGE_RECOLOR_OPA(val) \
    { \
        .prop = LV_STYLE_IMAGE_RECOLOR_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_LINE_WIDTH(val) \
    { \
        .prop = LV_STYLE_LINE_WIDTH, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_LINE_DASH_WIDTH(val) \
    { \
        .prop = LV_STYLE_LINE_DASH_WIDTH, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_LINE_DASH_GAP(val) \
    { \
        .prop = LV_STYLE_LINE_DASH_GAP, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_LINE_ROUNDED(val) \
    { \
        .prop = LV_STYLE_LINE_ROUNDED, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_LINE_COLOR(val) \
    { \
        .prop = LV_STYLE_LINE_COLOR, .value = { .color = val } \
    }

#define LV_STYLE_CONST_LINE_OPA(val) \
    { \
        .prop = LV_STYLE_LINE_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_ARC_WIDTH(val) \
    { \
        .prop = LV_STYLE_ARC_WIDTH, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_ARC_ROUNDED(val) \
    { \
        .prop = LV_STYLE_ARC_ROUNDED, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_ARC_COLOR(val) \
    { \
        .prop = LV_STYLE_ARC_COLOR, .value = { .color = val } \
    }

#define LV_STYLE_CONST_ARC_OPA(val) \
    { \
        .prop = LV_STYLE_ARC_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_ARC_IMAGE_SRC(val) \
    { \
        .prop = LV_STYLE_ARC_IMAGE_SRC, .value = { .ptr = val } \
    }

#define LV_STYLE_CONST_TEXT_COLOR(val) \
    { \
        .prop = LV_STYLE_TEXT_COLOR, .value = { .color = val } \
    }

#define LV_STYLE_CONST_TEXT_OPA(val) \
    { \
        .prop = LV_STYLE_TEXT_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TEXT_FONT(val) \
    { \
        .prop = LV_STYLE_TEXT_FONT, .value = { .ptr = val } \
    }

#define LV_STYLE_CONST_TEXT_LETTER_SPACE(val) \
    { \
        .prop = LV_STYLE_TEXT_LETTER_SPACE, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TEXT_LINE_SPACE(val) \
    { \
        .prop = LV_STYLE_TEXT_LINE_SPACE, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TEXT_DECOR(val) \
    { \
        .prop = LV_STYLE_TEXT_DECOR, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TEXT_ALIGN(val) \
    { \
        .prop = LV_STYLE_TEXT_ALIGN, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TEXT_OUTLINE_STROKE_COLOR(val) \
    { \
        .prop = LV_STYLE_TEXT_OUTLINE_STROKE_COLOR, .value = { .color = val } \
    }

#define LV_STYLE_CONST_TEXT_OUTLINE_STROKE_WIDTH(val) \
    { \
        .prop = LV_STYLE_TEXT_OUTLINE_STROKE_WIDTH, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TEXT_OUTLINE_STROKE_OPA(val) \
    { \
        .prop = LV_STYLE_TEXT_OUTLINE_STROKE_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_RADIUS(val) \
    { \
        .prop = LV_STYLE_RADIUS, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_RADIAL_OFFSET(val) \
    { \
        .prop = LV_STYLE_RADIAL_OFFSET, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_CLIP_CORNER(val) \
    { \
        .prop = LV_STYLE_CLIP_CORNER, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_OPA(val) \
    { \
        .prop = LV_STYLE_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_OPA_LAYERED(val) \
    { \
        .prop = LV_STYLE_OPA_LAYERED, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_COLOR_FILTER_DSC(val) \
    { \
        .prop = LV_STYLE_COLOR_FILTER_DSC, .value = { .ptr = val } \
    }

#define LV_STYLE_CONST_COLOR_FILTER_OPA(val) \
    { \
        .prop = LV_STYLE_COLOR_FILTER_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_RECOLOR(val) \
    { \
        .prop = LV_STYLE_RECOLOR, .value = { .color = val } \
    }

#define LV_STYLE_CONST_RECOLOR_OPA(val) \
    { \
        .prop = LV_STYLE_RECOLOR_OPA, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_ANIM(val) \
    { \
        .prop = LV_STYLE_ANIM, .value = { .ptr = val } \
    }

#define LV_STYLE_CONST_ANIM_DURATION(val) \
    { \
        .prop = LV_STYLE_ANIM_DURATION, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_TRANSITION(val) \
    { \
        .prop = LV_STYLE_TRANSITION, .value = { .ptr = val } \
    }

#define LV_STYLE_CONST_BLEND_MODE(val) \
    { \
        .prop = LV_STYLE_BLEND_MODE, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_LAYOUT(val) \
    { \
        .prop = LV_STYLE_LAYOUT, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BASE_DIR(val) \
    { \
        .prop = LV_STYLE_BASE_DIR, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_BITMAP_MASK_SRC(val) \
    { \
        .prop = LV_STYLE_BITMAP_MASK_SRC, .value = { .ptr = val } \
    }

#define LV_STYLE_CONST_ROTARY_SENSITIVITY(val) \
    { \
        .prop = LV_STYLE_ROTARY_SENSITIVITY, .value = { .num = (int32_t)val } \
    }
#if LV_USE_FLEX

#define LV_STYLE_CONST_FLEX_FLOW(val) \
    { \
        .prop = LV_STYLE_FLEX_FLOW, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_FLEX_MAIN_PLACE(val) \
    { \
        .prop = LV_STYLE_FLEX_MAIN_PLACE, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_FLEX_CROSS_PLACE(val) \
    { \
        .prop = LV_STYLE_FLEX_CROSS_PLACE, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_FLEX_TRACK_PLACE(val) \
    { \
        .prop = LV_STYLE_FLEX_TRACK_PLACE, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_FLEX_GROW(val) \
    { \
        .prop = LV_STYLE_FLEX_GROW, .value = { .num = (int32_t)val } \
    }
#endif /*LV_USE_FLEX*/

#if LV_USE_GRID

#define LV_STYLE_CONST_GRID_COLUMN_DSC_ARRAY(val) \
    { \
        .prop = LV_STYLE_GRID_COLUMN_DSC_ARRAY, .value = { .ptr = val } \
    }

#define LV_STYLE_CONST_GRID_COLUMN_ALIGN(val) \
    { \
        .prop = LV_STYLE_GRID_COLUMN_ALIGN, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_GRID_ROW_DSC_ARRAY(val) \
    { \
        .prop = LV_STYLE_GRID_ROW_DSC_ARRAY, .value = { .ptr = val } \
    }

#define LV_STYLE_CONST_GRID_ROW_ALIGN(val) \
    { \
        .prop = LV_STYLE_GRID_ROW_ALIGN, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_GRID_CELL_COLUMN_POS(val) \
    { \
        .prop = LV_STYLE_GRID_CELL_COLUMN_POS, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_GRID_CELL_X_ALIGN(val) \
    { \
        .prop = LV_STYLE_GRID_CELL_X_ALIGN, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_GRID_CELL_COLUMN_SPAN(val) \
    { \
        .prop = LV_STYLE_GRID_CELL_COLUMN_SPAN, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_GRID_CELL_ROW_POS(val) \
    { \
        .prop = LV_STYLE_GRID_CELL_ROW_POS, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_GRID_CELL_Y_ALIGN(val) \
    { \
        .prop = LV_STYLE_GRID_CELL_Y_ALIGN, .value = { .num = (int32_t)val } \
    }

#define LV_STYLE_CONST_GRID_CELL_ROW_SPAN(val) \
    { \
        .prop = LV_STYLE_GRID_CELL_ROW_SPAN, .value = { .num = (int32_t)val } \
    }
#endif /*LV_USE_GRID*/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_STYLE_GEN_H */
