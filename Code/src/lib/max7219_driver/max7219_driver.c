
#include "max7219_driver.h"
#include "hardware/gpio.h"



void max7219_init(max7219_t *device,
                   spi_inst_t *spi_instance,
                   uint8_t load_pin,
                   uint8_t clk_pin,
                   uint8_t mosi_pin,
                   uint8_t num_digits) {

    device->spi_instance = spi_instance;
    device->load_pin = load_pin;
    device->clk_pin = clk_pin;
    device->mosi_pin = mosi_pin;
    device->num_digits = num_digits;

    // max7219 supports up to 10MHz
    spi_init(device->spi_instance, 10000 * 1000)
    spi_set_format(device->spi_instance, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    gpio_set_function(device->clk_pin, GPIO_FUNC_SPI);
    gpio_set_function(device->mosi_pin, GPIO_FUNC_SPI);

    gpio_init(device->load_pin);
    gpio_set_dir(device->load_pin, GPIO_OUT);
    gpio_put(device->load_pin, 1);


    max7219_send_command(device, 0x0F, 0x00);   // turn off test mode
    max7219_send_command(device, 0x0B, device->num_digits - 1 );    // set scan limit to x digits
    max7219_send_command(device, 0x0A, 0x00);   // set brightness to minimum (0)
    max7219_send_command(device, 0x0C, 0x01);   // move device out of shutdown mode
}


void max7219_send_command(max7219_t *device, uint8_t address, uint8_t data) {
    uint8_t tx_buf[2] = {address, data};

    gpio_put(device->load_pin, 0);
    spi_write_blocking(device->spi_instance, tx_buf, 2);
    gpio_put(device->load_pin, 1);
}


void max7219_set_decode_mode(max7219_t *device, max7219_decode_mode_t mode) {

    // mode = 1 when mode = BCD_DECODE
    if (mode) {         // BCD_DECODE
        max7219_send_command(device, 0x09, 0xFF);
    } else {            // NO_DECODE
        max7219_send_command(device, 0x09, 0x00);
    }
}

void max7219_display_number(max7219_t *device,
                            uint8_t segment,
                            uint8_t number) {
    max7219_send_command(device, segment + 1, number);
}


void max7219_display_number_2_digits(max7219_t *device,
                            uint8_t number) {

    if (number > 99) {
        max7219_send_command(device, 0x01, 0x00);
        max7219_send_command(device, 0x02, 0x00);
        return;
    }

    max7219_display_number(device, 0, number / 10);
    max7219_display_number(device, 1, number % 10);
}

