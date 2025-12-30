/**
 * @file lvgl_display.h
 * @brief LVGL display driver for RP2040 MIDI Controller
 * 
 * This is a placeholder display driver that provides the necessary
 * interface for LVGL to work. Actual display hardware support can be
 * added later.
 */

#ifndef LVGL_DISPLAY_H
#define LVGL_DISPLAY_H

#include "lvgl.h"

/** Display width in pixels */
#define DISP_HOR_RES 240

/** Display height in pixels */
#define DISP_VER_RES 240

/**
 * @brief Initialize LVGL display driver
 * @return Pointer to display object or NULL on failure
 */
lv_display_t* lvgl_display_init(void);

/**
 * @brief Get current display object
 * @return Pointer to display object or NULL if not initialized
 */
lv_display_t* lvgl_display_get(void);

#endif /* LVGL_DISPLAY_H */
