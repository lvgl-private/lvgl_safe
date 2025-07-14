/**
 * @file lvgl.h
 * Include all LVGL related headers
 */

#ifndef LVGL_H
#define LVGL_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************
 * CURRENT VERSION OF LVGL
 ***************************/
#include "lv_version.h"

/*********************
 *      INCLUDES
 *********************/
#include "src/lv_init.h"

#include "src/stdlib/lv_mem.h"
#include "src/stdlib/lv_string.h"
#include "src/stdlib/lv_sprintf.h"

#include "src/misc/lv_log.h"
#include "src/misc/lv_timer.h"
#include "src/misc/lv_math.h"
#include "src/misc/lv_array.h"
#include "src/misc/lv_async.h"
#include "src/misc/lv_anim_timeline.h"
#include "src/misc/lv_profiler_builtin.h"
#include "src/misc/lv_rb.h"
#include "src/misc/lv_utils.h"
#include "src/misc/lv_iter.h"
#include "src/misc/lv_circle_buf.h"
#include "src/misc/lv_tree.h"
#include "src/misc/cache/lv_cache.h"

#include "src/tick/lv_tick.h"

#include "src/core/lv_obj.h"
#include "src/core/lv_group.h"
#include "src/indev/lv_indev.h"
#include "src/indev/lv_indev_gesture.h"
#include "src/core/lv_refr.h"
#include "src/display/lv_display.h"

#include "src/font/lv_font.h"
#include "src/font/lv_binfont_loader.h"
#include "src/font/lv_font_fmt_txt.h"

#include "src/widgets/bar/lv_bar.h"
#include "src/widgets/button/lv_button.h"
#include "src/widgets/canvas/lv_canvas.h"
#include "src/widgets/checkbox/lv_checkbox.h"
#include "src/widgets/image/lv_image.h"
#include "src/widgets/label/lv_label.h"
#include "src/widgets/led/lv_led.h"
#include "src/widgets/slider/lv_slider.h"
#include "src/widgets/switch/lv_switch.h"

#include "src/others/snapshot/lv_snapshot.h"
#include "src/others/sysmon/lv_sysmon.h"
#include "src/others/monkey/lv_monkey.h"
#include "src/others/gridnav/lv_gridnav.h"
#include "src/others/fragment/lv_fragment.h"
#include "src/others/imgfont/lv_imgfont.h"
#include "src/others/observer/lv_observer.h"
#include "src/others/ime/lv_ime_pinyin.h"
#include "src/others/file_explorer/lv_file_explorer.h"
#include "src/others/font_manager/lv_font_manager.h"
#include "src/others/translation/lv_translation.h"
#include "src/others/xml/lv_xml.h"
#include "src/others/test/lv_test.h"

#include "src/libs/bin_decoder/lv_bin_decoder.h"
#include "src/libs/fsdrv/lv_fsdrv.h"

#include "src/draw/lv_draw_buf.h"
#include "src/draw/lv_draw_vector.h"

#include "src/themes/lv_theme.h"

#include "src/drivers/lv_drivers.h"


#if LV_USE_PRIVATE_API
#include "src/lvgl_private.h"
#endif


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

/** Gives 1 if the x.y.z version is supported in the current version
 * Usage:
 *
 * - Require v6
 * #if LV_VERSION_CHECK(6,0,0)
 *   new_func_in_v6();
 * #endif
 *
 *
 * - Require at least v5.3
 * #if LV_VERSION_CHECK(5,3,0)
 *   new_feature_from_v5_3();
 * #endif
 *
 *
 * - Require v5.3.2 bugfixes
 * #if LV_VERSION_CHECK(5,3,2)
 *   bugfix_in_v5_3_2();
 * #endif
 *
 */
#define LV_VERSION_CHECK(x,y,z) (x == LVGL_VERSION_MAJOR && (y < LVGL_VERSION_MINOR || (y == LVGL_VERSION_MINOR && z <= LVGL_VERSION_PATCH)))

/**
 * Wrapper functions for VERSION macros
 */

static inline int lv_version_major(void)
{
    return LVGL_VERSION_MAJOR;
}

static inline int lv_version_minor(void)
{
    return LVGL_VERSION_MINOR;
}

static inline int lv_version_patch(void)
{
    return LVGL_VERSION_PATCH;
}

static inline const char * lv_version_info(void)
{
    return LVGL_VERSION_INFO;
}

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_H*/
