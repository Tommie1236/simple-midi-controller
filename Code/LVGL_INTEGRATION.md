# LVGL Integration for Simple MIDI Controller

This document describes the LVGL (Light and Versatile Graphics Library) integration for the Simple MIDI Controller project.

## Overview

LVGL v9.4.0 has been integrated into this project to provide graphical display capabilities. This integration provides a foundation for adding a graphical menu system and enhanced visual feedback beyond the MAX7219 7-segment display.

## Components

### 1. LVGL Library
- Added as a git submodule in `Code/lib/lvgl`
- Configured via `Code/lv_conf.h`
- Built as part of the main firmware build

### 2. Display Driver (`lvgl_display.c` / `lvgl_display.h`)
- Provides the interface between LVGL and display hardware
- Currently implements a stub driver that can be extended for actual display hardware
- Default resolution: 240x240 pixels (configurable in `lvgl_display.h`)
- Uses partial buffering for memory efficiency

### 3. UI Integration (`main.c`)
- LVGL initialization in `init_lvgl()`
- Periodic update via `lvgl_task()` called in main loop
- Example UI showing current bank number
- Bank number updates when bank buttons are pressed

## Configuration

The LVGL configuration file (`Code/lv_conf.h`) is optimized for the RP2040:

- **Color Depth**: 16-bit (RGB565)
- **Memory**: Uses standard C library malloc
- **Rendering**: Software rendering with no SIMD acceleration
- **Widgets**: Only essential widgets enabled (Label, Button, Arc, Bar, Image)
- **Demos/Examples**: Disabled to save flash space
- **Font**: Montserrat 14 enabled by default

## Memory Usage

The current firmware with LVGL uses approximately:
- Flash: 393KB (~18.76% of 2MB)
- RAM: 25KB (~9.85% of 256KB)

LVGL display buffers use approximately 4.8KB (2 buffers of 240x10 pixels @ 16bpp).

## Display Hardware Support

The stub display driver (`lvgl_display.c`) provides the framework for connecting actual display hardware. To add display support:

1. Implement the `disp_flush_cb()` function to send pixel data to your display
2. Add display initialization code in `lvgl_display_init()`
3. Configure display resolution in `lvgl_display.h` (`DISP_HOR_RES`, `DISP_VER_RES`)
4. Add required SPI/I2C/parallel interface code

### Supported Display Types
Common display controllers that can be integrated:
- **ST7789** - Popular 240x240 TFT LCD
- **ILI9341** - 320x240 TFT LCD
- **SSD1306** - 128x64 OLED (monochrome)
- **ST7735** - Small TFT LCDs

## API

### Display Initialization
```c
lv_display_t* lvgl_display_init(void);
```
Initializes LVGL and creates the display object.

### Display Task
```c
void lvgl_task(void);
```
Call this periodically (in main loop) to handle LVGL timers and rendering.

### Update Bank Display
```c
void lvgl_update_bank(uint8_t bank);
```
Updates the displayed bank number on the LVGL UI.

## Building

The build process automatically includes LVGL:

```bash
./build.sh
```

The ARM GCC compiler (`gcc-arm-none-eabi`) is required.

## Known Limitations

1. **No Physical Display**: The current implementation is a stub driver. No actual display hardware is connected.
2. **Helium ASM Disabled**: ARM Helium SIMD assembly optimizations are disabled (not supported on RP2040's Cortex-M0+).
3. **Limited Widgets**: Only essential widgets are enabled to conserve flash space.

## Future Enhancements

- Add support for specific display hardware (e.g., ST7789)
- Implement touch input handling
- Create more sophisticated UI for bank selection and settings
- Add configuration menu
- Implement display themes and animations

## Troubleshooting

### Build Errors

**"ARM Helium assembly errors"**: The helium assembly file has been disabled by renaming `lv_blend_helium.S` to `lv_blend_helium.S.disabled`. This is expected for RP2040.

**"lv_conf.h not found"**: Ensure `Code/lv_conf.h` exists and `LV_CONF_INCLUDE_SIMPLE` is defined properly.

### Runtime Issues

**"No visual output"**: This is expected - the stub driver doesn't output to actual hardware. Connect a physical display and implement the flush callback.

## Resources

- [LVGL Official Documentation](https://docs.lvgl.io/)
- [LVGL GitHub Repository](https://github.com/lvgl/lvgl)
- [RP2040 Datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
