/**
 * @file lvgl_private.h
 *
 */

#ifndef LVGL_PRIVATE_H
#define LVGL_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "src/core/lv_global.h"

#include "src/display/lv_display_private.h"
#include "src/indev/lv_indev_private.h"
#include "src/misc/lv_text_private.h"
#include "src/misc/cache/lv_cache_entry_private.h"
#include "src/misc/cache/lv_cache_private.h"
#include "src/stdlib/lv_mem_private.h"
#include "src/others/file_explorer/lv_file_explorer_private.h"
#include "src/others/sysmon/lv_sysmon_private.h"
#include "src/others/monkey/lv_monkey_private.h"
#include "src/others/ime/lv_ime_pinyin_private.h"
#include "src/others/fragment/lv_fragment_private.h"
#include "src/others/observer/lv_observer_private.h"
#include "src/others/xml/lv_xml_private.h"
#include "src/draw/lv_draw_triangle_private.h"
#include "src/draw/lv_draw_private.h"
#include "src/draw/lv_draw_rect_private.h"
#include "src/draw/lv_draw_image_private.h"
#include "src/draw/lv_image_decoder_private.h"
#include "src/draw/lv_draw_label_private.h"
#include "src/draw/lv_draw_vector_private.h"
#include "src/draw/lv_draw_buf_private.h"
#include "src/draw/lv_draw_mask_private.h"
#include "src/draw/sw/lv_draw_sw_private.h"
#include "src/draw/sw/lv_draw_sw_mask_private.h"
#include "src/draw/sw/blend/lv_draw_sw_blend_private.h"
#include "src/drivers/libinput/lv_xkb_private.h"
#include "src/drivers/libinput/lv_libinput_private.h"
#include "src/drivers/evdev/lv_evdev_private.h"
#include "src/font/lv_font_fmt_txt_private.h"
#include "src/themes/lv_theme_private.h"
#include "src/core/lv_refr_private.h"
#include "src/core/lv_obj_style_private.h"
#include "src/core/lv_obj_private.h"
#include "src/core/lv_obj_scroll_private.h"
#include "src/core/lv_obj_draw_private.h"
#include "src/core/lv_obj_class_private.h"
#include "src/core/lv_group_private.h"
#include "src/core/lv_obj_event_private.h"
#include "src/misc/lv_timer_private.h"
#include "src/misc/lv_area_private.h"
#include "src/misc/lv_fs_private.h"
#include "src/misc/lv_profiler_builtin_private.h"
#include "src/misc/lv_event_private.h"
#include "src/misc/lv_bidi_private.h"
#include "src/misc/lv_rb_private.h"
#include "src/misc/lv_style_private.h"
#include "src/misc/lv_color_op_private.h"
#include "src/misc/lv_anim_private.h"
#include "src/widgets/slider/lv_slider_private.h"
#include "src/widgets/switch/lv_switch_private.h"
#include "src/widgets/bar/lv_bar_private.h"
#include "src/widgets/image/lv_image_private.h"
#include "src/widgets/checkbox/lv_checkbox_private.h"
#include "src/widgets/button/lv_button_private.h"
#include "src/widgets/led/lv_led_private.h"
#include "src/widgets/label/lv_label_private.h"
#include "src/widgets/canvas/lv_canvas_private.h"
#include "src/tick/lv_tick_private.h"
#include "src/stdlib/builtin/lv_tlsf_private.h"
#include "src/osal/lv_os_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRIVATE_H*/
