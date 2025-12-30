/**
 * @file lvgl_display.c
 * @brief LVGL display driver for RP2040 MIDI Controller
 */

#include "lvgl_display.h"
#include "pico/stdlib.h"
#include <string.h>

/* Display driver instance */
static lv_display_t* disp = NULL;

/* Display buffers - double buffering for smooth updates */
static lv_color_t disp_buf1[DISP_HOR_RES * 10];  /* Partial buffer 1 */
static lv_color_t disp_buf2[DISP_HOR_RES * 10];  /* Partial buffer 2 */

/**
 * @brief Flush display callback - called when LVGL wants to update display
 * 
 * This is a stub implementation. When actual display hardware is connected,
 * this function should send the pixel data to the display.
 * 
 * @param disp_drv Display driver
 * @param area Area to update
 * @param px_map Pixel data buffer
 */
static void disp_flush_cb(lv_display_t* disp_drv, const lv_area_t* area, uint8_t* px_map)
{
    /* TODO: Send pixel data to actual display hardware
     * For now, just mark the flush as ready */
    
    /* Inform LVGL that flush is ready */
    lv_display_flush_ready(disp_drv);
}

lv_display_t* lvgl_display_init(void)
{
    /* Create display object */
    disp = lv_display_create(DISP_HOR_RES, DISP_VER_RES);
    if (disp == NULL) {
        return NULL;
    }
    
    /* Set display buffers - using double buffering for smoother updates */
    lv_display_set_buffers(disp, disp_buf1, disp_buf2, 
                          sizeof(disp_buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    
    /* Set flush callback */
    lv_display_set_flush_cb(disp, disp_flush_cb);
    
    return disp;
}

lv_display_t* lvgl_display_get(void)
{
    return disp;
}
