#include "main.h"

	
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "stdio.h"
#include "bitset"

#include "bsp/board.h"
#include "tusb.h"



void midi_task(void);
void key_matrix_task(void);
void segment_display_task(void);
void init_gpio(void);

uint32_t tud_midi_n_stream_write(uint8_t itf, uint8_t cable_num, uint8_t const* buffer, uint32_t bufsize);  // vscode thinks this isn't defined, but is defined at compile and complies correctly. //TODO: remove when fixed or finished

uint8_t bank = 0; 
std::bitset<32> buttons_pressed_set = 0x00000000;
uint32_t buttons_pressed = 0x00000000;
uint32_t previous_buttons_pressed = 0x00000000;



void core1_main(){
    while (1){
        tud_task();
    }
};

void core0_main(){
    board_init(); 
    tusb_init();
    stdio_usb_init();

    init_gpio();

    multicore_launch_core1(&core1_main);

    while(1) {
        key_matrix_task();
        midi_task();
        //segment_display_task();
	};
};


int main() {
	
	core0_main();

    return 0;
}



void init_gpio() {
    gpio_init_mask(matrix_out_mask | matrix_in_mask);
    gpio_set_function_masked(matrix_out_mask | matrix_in_mask, GPIO_FUNC_SIO);
    gpio_init(BANK_UP_PIN);
    gpio_init(BANK_DOWN_PIN);

    gpio_set_dir_in_masked(matrix_in_mask); 
    gpio_set_dir_out_masked(matrix_out_mask);

    gpio_set_dir(BANK_UP_PIN, GPIO_IN);
    gpio_set_dir(BANK_DOWN_PIN, GPIO_IN); 
    gpio_pull_down(BANK_UP_PIN);
    gpio_pull_down(BANK_DOWN_PIN);

    

    // segment display
    //gpio_set_function(MAX7219_CS_PIN, GPIO_FUNC_SIO);
    //gpio_set_function(MAX7219_CLK_PIN, GPIO_FUNC_SPI);
    //gpio_set_function(MAX7219_MOSI_PIN, GPIO_FUNC_SPI);
    //gpio_set_dir(MAX7219_CS_PIN, GPIO_OUT);

    //spi_init(MAX7219_SPI_PORT, 10000 * 1000); // 10MHz

    // phiysical midi        //TODO: maybe support later
    // gpio_set_function(MIDI_TX_PIN, GPIO_FUNC_UART);
    // gpio_set_function(MIDI_RX_PIN, GPIO_FUNC_UART);

}

void midi_task() {

    uint8_t msg[3];
    buttons_pressed = buttons_pressed_set.to_ulong();
    uint32_t changed_buttons = buttons_pressed ^ previous_buttons_pressed;

    msg[0] = 0x90; // Note on - Channel 1 TODO: apply bank

    for (int i = 0; i < 32; i++) {
        uint32_t mask = 1 << i;

        if (changed_buttons & mask) {
            
            msg[1] = i; // Note number TODO: apply bank
            if (buttons_pressed & mask) {
                // Button pressed
                msg[2] = 127; // velocito / on
                printf("Button %d pressed\n", i);
            } else {
                // Button released
                msg[2] = 0; // velocito / off
                printf("Button %d released\n", i);
            }
            tud_midi_n_stream_write(0, 0, msg, 3);
        }

    }

    previous_buttons_pressed = buttons_pressed;
}

void key_matrix_task() {
    // TODO: implement bank buttons 
    // TODO: support gpio remmaping
    
    gpio_put(COL_A_PIN, 1);
    sleep_ms(10);
    buttons_pressed_set.set(0, gpio_get(ROW_0_PIN));
    buttons_pressed_set.set(8, gpio_get(ROW_1_PIN));
    buttons_pressed_set.set(16, gpio_get(ROW_2_PIN));
    buttons_pressed_set.set(24, gpio_get(ROW_3_PIN));
    gpio_put(COL_A_PIN, 0);
    gpio_put(COL_B_PIN, 1);
    sleep_ms(10);
    buttons_pressed_set.set(1, gpio_get(ROW_0_PIN));
    buttons_pressed_set.set(9, gpio_get(ROW_1_PIN));
    buttons_pressed_set.set(17, gpio_get(ROW_2_PIN));
    buttons_pressed_set.set(25, gpio_get(ROW_3_PIN));
    gpio_put(COL_B_PIN, 0);
    gpio_put(COL_C_PIN, 1);
    sleep_ms(10);
    buttons_pressed_set.set(2, gpio_get(ROW_0_PIN));
    buttons_pressed_set.set(10, gpio_get(ROW_1_PIN));
    buttons_pressed_set.set(18, gpio_get(ROW_2_PIN));
    buttons_pressed_set.set(26, gpio_get(ROW_3_PIN));
    gpio_put(COL_C_PIN, 0);
    gpio_put(COL_D_PIN, 1);
    sleep_ms(10);
    buttons_pressed_set.set(3, gpio_get(ROW_0_PIN));
    buttons_pressed_set.set(11, gpio_get(ROW_1_PIN));
    buttons_pressed_set.set(19, gpio_get(ROW_2_PIN));
    buttons_pressed_set.set(27, gpio_get(ROW_3_PIN));
    gpio_put(COL_D_PIN, 0);
    gpio_put(COL_E_PIN, 1);
    sleep_ms(10);
    buttons_pressed_set.set(4, gpio_get(ROW_0_PIN));
    buttons_pressed_set.set(12, gpio_get(ROW_1_PIN));
    buttons_pressed_set.set(20, gpio_get(ROW_2_PIN));
    buttons_pressed_set.set(28, gpio_get(ROW_3_PIN));
    gpio_put(COL_E_PIN, 0);
    gpio_put(COL_F_PIN, 1);
    sleep_ms(10);
    buttons_pressed_set.set(5, gpio_get(ROW_0_PIN));
    buttons_pressed_set.set(13, gpio_get(ROW_1_PIN));
    buttons_pressed_set.set(21, gpio_get(ROW_2_PIN));
    buttons_pressed_set.set(29, gpio_get(ROW_3_PIN));
    gpio_put(COL_F_PIN, 0);
    gpio_put(COL_G_PIN, 1);
    sleep_ms(10);
    buttons_pressed_set.set(6, gpio_get(ROW_0_PIN));
    buttons_pressed_set.set(14, gpio_get(ROW_1_PIN));
    buttons_pressed_set.set(22, gpio_get(ROW_2_PIN));
    buttons_pressed_set.set(30, gpio_get(ROW_3_PIN));
    gpio_put(COL_G_PIN, 0);
    gpio_put(COL_H_PIN, 1);
    sleep_ms(10);
    buttons_pressed_set.set(7, gpio_get(ROW_0_PIN));
    buttons_pressed_set.set(15, gpio_get(ROW_1_PIN));
    buttons_pressed_set.set(23, gpio_get(ROW_2_PIN));
    buttons_pressed_set.set(31, gpio_get(ROW_3_PIN));
    gpio_put(COL_H_PIN, 0);




}
