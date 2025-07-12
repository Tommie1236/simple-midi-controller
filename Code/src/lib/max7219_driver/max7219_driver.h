#ifndef MAX7219_DRIVER_H
#define MAX7219_DRIVER_H


#include "pico/stdlib.h"
#include "hardware/spi.h"

typedef enum {
    NO_DECODE,
    BCD_DECODE
} max7219_decode_mode_t;

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

void max7219_send_command(max7219_t *device,
                          uint8_t address,
                          uint8_t data);

void max7219_set_decode_mode(max7219_t *device,
                             max7219_decode_mode_t mode);

void max7219_display_number(max7219_t *device,
                            uint8_t segment,
                            uint8_t number);

void max7219_display_number_2_digits(max7219_t *device,
                            uint8_t number);


#endif // MAX7219_DRIVER_H
