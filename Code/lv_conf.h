/**
 * @file lv_conf.h
 * Configuration file for LVGL v9.x
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/

/** Color depth: 1 (I1), 8 (L8), 16 (RGB565), 24 (RGB888), 32 (XRGB8888) */
#define LV_COLOR_DEPTH 16

/*=========================
   STDLIB WRAPPER SETTINGS
 *=========================*/

#define LV_USE_STDLIB_MALLOC    LV_STDLIB_CLIB
#define LV_USE_STDLIB_STRING    LV_STDLIB_CLIB
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_CLIB

/*====================
   HAL SETTINGS
 *====================*/

/** Default display refresh, input device read and animation step period. */
#define LV_DEF_REFR_PERIOD  33      /**< [ms] */

/** Default Dots Per Inch. Used to initialize default sizes such as widgets sized, style paddings. */
#define LV_DPI_DEF 130              /**< [px/inch] */

/*=================
 * OPERATING SYSTEM
 *=================*/
#define LV_USE_OS   LV_OS_NONE

/*==================
 * RENDERING CONFIG
 *==================*/

/** Align the stride of all layers and images.
 * - LV_DRAW_BUF_STRIDE_ALIGN = 1: Means stride can be any number.
 * - LV_DRAW_BUF_STRIDE_ALIGN = 64: Means stride should be aligned to 64 bytes.
 */
#define LV_DRAW_BUF_STRIDE_ALIGN                1

/** Align the start address of draw_buf addresses.
 * - LV_DRAW_BUF_ALIGN = 1: Means stride can be at any address.
 * - LV_DRAW_BUF_ALIGN = 64: Means stride should be aligned to 64 bytes.
 */
#define LV_DRAW_BUF_ALIGN                       4

/** Use ARM Helium SIMD acceleration (only for compatible ARM cores) */
#define LV_USE_DRAW_SW_ASM              LV_DRAW_SW_ASM_NONE

/*=================
 * FONT USAGE
 *=================*/

/** Montserrat fonts with ASCII range and some symbols using bpp = 4
 * https://fonts.google.com/specimen/Montserrat  */
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 0
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/** Demonstrate special features */
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0  /**< bpp = 3 */
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0  /**< Hebrew, Arabic, Persian letters and all their forms */
#define LV_FONT_SIMSUN_16_CJK            0  /**< 1000 most common CJK radicals */

/** Pixel perfect monospace fonts */
#define LV_FONT_UNSCII_8  0
#define LV_FONT_UNSCII_16 0

/** Optionally declare custom fonts here. */
#define LV_FONT_CUSTOM_DECLARE

/** Enable/disable support for the built-in fonts. */
#define LV_FONT_FMT_TXT_LARGE 0

/** Default font */
#define LV_FONT_DEFAULT &lv_font_montserrat_14

/*=================
 * TEXT SETTINGS
 *=================*/

/** Select a character encoding for strings.
 * Your IDE or editor should have the same character encoding
 * - LV_TXT_ENC_UTF8
 * - LV_TXT_ENC_ASCII
 */
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/*=================
 * WIDGET USAGE
 *=================*/

/** Enable all widget by default. Disable them individually if needed. */
#define LV_USE_WIDGETS 1

#if LV_USE_WIDGETS
    #define LV_USE_ANIMIMG      0
    #define LV_USE_ARC          1
    #define LV_USE_BAR          1
    #define LV_USE_BUTTON       1
    #define LV_USE_BUTTONMATRIX 0
    #define LV_USE_CALENDAR     0
    #define LV_USE_CANVAS       0
    #define LV_USE_CHART        0
    #define LV_USE_CHECKBOX     0
    #define LV_USE_DROPDOWN     0
    #define LV_USE_GRID         0
    #define LV_USE_IMAGE        1
    #define LV_USE_IMAGEBUTTON  0
    #define LV_USE_KEYBOARD     0
    #define LV_USE_LABEL        1
    #define LV_USE_LED          0
    #define LV_USE_LINE         0
    #define LV_USE_LIST         0
    #define LV_USE_MENU         0
    #define LV_USE_MSGBOX       0
    #define LV_USE_ROLLER       0
    #define LV_USE_SCALE        0
    #define LV_USE_SLIDER       0
    #define LV_USE_SPAN         0
    #define LV_USE_SPANGROUP    0
    #define LV_USE_SPINBOX      0
    #define LV_USE_SPINNER      0
    #define LV_USE_SWITCH       0
    #define LV_USE_TABLE        0
    #define LV_USE_TABVIEW      0
    #define LV_USE_TEXTAREA     0
    #define LV_USE_TILEVIEW     0
    #define LV_USE_WIN          0
#endif

/*==================
 * THEMES
 *==================*/

#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT
    #define LV_THEME_DEFAULT_DARK 0
    #define LV_THEME_DEFAULT_GROW 1
    #define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif

/*==================
 * LAYOUTS
 *==================*/

#define LV_USE_FLEX 0
#define LV_USE_GRID 0

/*====================
 * 3RD PARTY LIBRARIES
 *====================*/

#define LV_USE_FS_STDIO 0
#define LV_USE_FS_POSIX 0
#define LV_USE_FS_WIN32 0
#define LV_USE_FS_FATFS 0
#define LV_USE_FS_MEMFS 0

/*==================
 * EXAMPLES
 *==================*/

/** Enable the examples to be built with the library */
#define LV_BUILD_EXAMPLES 0

/*===================
 * DEMO USAGE
 *==================*/

#define LV_USE_DEMO_WIDGETS   0
#define LV_USE_DEMO_KEYPAD_AND_ENCODER 0
#define LV_USE_DEMO_BENCHMARK 0
#define LV_USE_DEMO_STRESS    0
#define LV_USE_DEMO_MUSIC     0
#define LV_USE_DEMO_FLEX_LAYOUT 0
#define LV_USE_DEMO_MULTILANG 0
#define LV_USE_DEMO_TRANSFORM 0
#define LV_USE_DEMO_SCROLL    0

/*==================
 * DEVICES
 *==================*/

#define LV_USE_LIBINPUT     0
#define LV_USE_EVDEV        0
#define LV_USE_LIBDRM       0
#define LV_USE_NUTTX        0
#define LV_USE_SDL          0
#define LV_USE_X11          0
#define LV_USE_WAYLAND      0
#define LV_USE_FBDEV        0

/*==================
 * DEBUGGING
 *==================*/

#define LV_USE_LOG 0
#if LV_USE_LOG
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
    #define LV_LOG_PRINTF 0
#endif

/*==================
 * ASSERTS
 *==================*/

#define LV_USE_ASSERT_NULL          1
#define LV_USE_ASSERT_MALLOC        1
#define LV_USE_ASSERT_STYLE         0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ           0

#define LV_ASSERT_HANDLER_INCLUDE <stdint.h>
#define LV_ASSERT_HANDLER while(1);

#endif /* LV_CONF_H */
