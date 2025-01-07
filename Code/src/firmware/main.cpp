#include "main.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

#include "bsp/board.h"
#include "tusb.h"



void midi_task(void);
void key_matrix_task(void);
void segment_display_task(void);
void init_gpio(void);

uint32_t tud_midi_n_stream_write(uint8_t itf, uint8_t cable_num, uint8_t const* buffer, uint32_t bufsize);  // vscode thinks this isn't defined, but is defined at compile and complies correctly. //TODO: remove when fixed or finished

uint8_t bank = 0; 
uint32_t buttons_pressed = 0x00000000;
uint32_t previous_buttons_pressed = 0x00000000;


int main() {
    board_init(); 

    init_gpio();

    tusb_init();

    while (1){
        tud_task();
        key_matrix_task();
        //segment_display_task();
        midi_task();
    }


    return 0;
}



void init_gpio() {

    // matrix and bank buttons
    uint32_t matrix_out_mask = (1 << ROW_0_PIN) | 
                               (1 << ROW_1_PIN) | 
                               (1 << ROW_2_PIN) | 
                               (1 << ROW_3_PIN);

    uint32_t matrix_in_mask =  (1 << COL_A_PIN) |
                               (1 << COL_B_PIN) |
                               (1 << COL_C_PIN) |
                               (1 << COL_D_PIN) |
                               (1 << COL_E_PIN) |
                               (1 << COL_F_PIN) |
                               (1 << COL_G_PIN) |
                               (1 << COL_H_PIN) |
                               (1 << BANK_UP_PIN) |
                               (1 << BANK_DOWN_PIN);

    gpio_set_dir_out_masked(matrix_out_mask); // set gpio 0-3 as outputs. 0-3 matrix rows
    gpio_set_dir_in_masked(matrix_in_mask); // set gpio 4-13 as inputs. 4-11 matrix columns, 12 bank up, 13 bank down
    for (int i = 4; i < 14; i++) {
        // set gpio pull down for inputs (gpio 4-13)
        gpio_pull_down(i);
    }
    
    // segment display
    gpio_set_function(MAX7219_CS_PIN, GPIO_FUNC_SIO);
    gpio_set_function(MAX7219_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MAX7219_DOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_dir(MAX7219_CS_PIN, GPIO_OUT);

    spi_init(MAX7219_SPI_PORT, 10000 * 1000); // 10MHz

    // phiysical midi        //TODO: maybe support later
    // gpio_set_function(MIDI_TX_PIN, GPIO_FUNC_UART);
    // gpio_set_function(MIDI_RX_PIN, GPIO_FUNC_UART);

}

void midi_task() {

    uint8_t msg[3];
    uint32_t changed_buttons = buttons_pressed ^ previous_buttons_pressed;

    msg[0] = 0x90; // Note on - Channel 1 TODO: apply bank

    for (int i = 0; i < 32; i++) {
        uint32_t mask = 1 << i;

        if (changed_buttons & mask) {
            
            msg[1] = i; // Note number TODO: apply bank
            if (buttons_pressed & mask) {
                // Buttont pressed
                msg[2] = 127; // velocito / on
            } else {
                // Button released
                msg[2] = 0; // velocito / off
            }
            tud_midi_n_stream_write(0, 0, msg, 3);
        }

    }

    previous_buttons_pressed = buttons_pressed;
}

void key_matrix_task() {
    // TODO: implement bank buttons 
    // TODO: support gpio remmaping
    const uint32_t matrix_out_mask = (1 << ROW_0_PIN) | 
                                     (1 << ROW_1_PIN) | 
                                     (1 << ROW_2_PIN) | 
                                     (1 << ROW_3_PIN);

    const uint32_t matrix_in_mask =  (1 << COL_A_PIN) |
                                     (1 << COL_B_PIN) |
                                     (1 << COL_C_PIN) |
                                     (1 << COL_D_PIN) |
                                     (1 << COL_E_PIN) |
                                     (1 << COL_F_PIN) |
                                     (1 << COL_G_PIN) |
                                     (1 << COL_H_PIN);

    gpio_put_masked(matrix_out_mask, 0); 
    gpio_put(ROW_0_PIN, 1);
    buttons_pressed = (gpio_get_all() & matrix_in_mask) << 4;
    gpio_put(ROW_0_PIN, 0);
    gpio_put(ROW_1_PIN, 1);
    buttons_pressed |= (gpio_get_all() & matrix_in_mask) >> 4;
    gpio_put(ROW_1_PIN, 0);
    gpio_put(ROW_2_PIN, 1);
    buttons_pressed |= (gpio_get_all() & matrix_in_mask) >> 12;
    gpio_put(ROW_2_PIN, 0);
    gpio_put(ROW_3_PIN, 1);
    buttons_pressed |= (gpio_get_all() & matrix_in_mask) >> 20;
}