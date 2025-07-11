#ifndef MAX7219_DRIVER_H
#define MAX7219_DRIVER_H


#include "pico/stdlib.h"
#include "hardware/spi.h"

typedef struct max7219 max7219_t;

struct max7219 {
    spi_inst_t *spi_instance;
    uint8_t load_pin;
    uint8_t clk_pin;
    uint8_t mosi_pin;
    uint8_t num_digits;
};

void max7219_init(max7219_t *device,
                  spi_inst_t *spi_instance,
                  uint8_t load_pin,
                  uint8_t clk_pin,
                  uint8_t mosi_pin,
                  uint8_t num_digits);

void max7219_send_command(max7219_t *device, uint8_t address, uint8_t data);

#endif // MAX7219_DRIVER_H
