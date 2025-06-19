#include "main.h"

	
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "stdio.h"
#include "cstdarg"
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

const uint8_t row_pins[MATRIX_ROWS] = {ROW_0_PIN, ROW_1_PIN, ROW_2_PIN, ROW_3_PIN};


#if DEBUG_MODE
#define debug_printf(format, ...) \
    do { \
        printf(format, ##__VA_ARGS__); \
    } while (0)
#else
#define debug_printf(format, ...) \
    do {} while (0)
#endif




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
	// implement banks later
    //gpio_init(BANK_UP_PIN);
    //gpio_init(BANK_DOWN_PIN);

    gpio_set_dir_in_masked(matrix_in_mask); 
	for (uint pin = 0; pin < 32; ++pin) {
		if (matrix_in_mask & (1u << pin)) {
			gpio_pull_up(pin);
		}
	}

    gpio_set_dir_out_masked(matrix_out_mask);

    //gpio_set_dir(BANK_UP_PIN, GPIO_IN);
    //gpio_set_dir(BANK_DOWN_PIN, GPIO_IN); 
    //gpio_pull_down(BANK_UP_PIN);
    //gpio_pull_down(BANK_DOWN_PIN);

    

    // segment display
    //gpio_set_function(MAX7219_CS_PIN, GPIO_FUNC_SIO);
    //gpio_set_function(MAX7219_CLK_PIN, GPIO_FUNC_SPI);
    //gpio_set_function(MAX7219_MOSI_PIN, GPIO_FUNC_SPI);
    //gpio_set_dir(MAX7219_CS_PIN, GPIO_OUT);

    //spi_init(MAX7219_SPI_PORT, 10000 * 1000); // 10MHz

    // physical midi        //TODO: maybe support later
    // gpio_set_function(MIDI_TX_PIN, GPIO_FUNC_UART);
    // gpio_set_function(MIDI_RX_PIN, GPIO_FUNC_UART);

}

void midi_task() {

    uint8_t msg[3];
    uint32_t changed_buttons = buttons_pressed ^ previous_buttons_pressed;
	
    // debug_printf("buttons: %08X \n", int(buttons_pressed));

    msg[0] = 0x90; // Note on - Channel 1 TODO: apply bank

    for (int i = 0; i < 32; i++) {
        uint32_t mask = 1 << i;

        if (changed_buttons & mask) {
            
            msg[1] = i; // Note number TODO: apply bank
            if (buttons_pressed & mask) {
                // Button pressed
                msg[2] = 127; // velocito / on
                debug_printf("Button %d pressed\n", i);
            } else {
                // Button released
                msg[2] = 0; // velocito / off
                debug_printf("Button %d released\n", i);
            }
            tud_midi_n_stream_write(0, 0, msg, 3);
        }

    }

    previous_buttons_pressed = buttons_pressed;
}

void key_matrix_task() {
    buttons_pressed = 0;

    for ( int row_idx = 0; row_idx < MATRIX_ROWS; ++row_idx) {
        
        gpio_set_mask(matrix_out_mask);

        gpio_put(row_pins[row_idx], 0);
        sleep_us(50);

        uint32_t gpio_state = gpio_get_all();

        uint8_t columns_byte = ~(uint8_t)((gpio_state & matrix_in_mask) >> 4 );
        columns_byte &= 0xFF;

        buttons_pressed |= ((uint8_t) columns_byte << (row_idx * MATRIX_COLS));
        gpio_put(row_pins[row_idx], 1);
    }

}
