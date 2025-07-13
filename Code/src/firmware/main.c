#include "main.h"

// rp2040 
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"

// c/cpp 
#include <string.h> // memcpy

// tinyusb
#include "bsp/board.h"
#include "tusb.h"

#include "max7219_driver.h"


void midi_task();
void key_matrix_task();
void init_gpio();
void init_segment_display();
void init_phisical_midi();
void check_debug();
void write_midi(uint8_t const* message, uint8_t lenght);

max7219_t display;

uint8_t current_bank = 0;

uint32_t buttons_pressed = 0x00000000;
uint32_t previous_buttons_pressed = 0x00000000;

uint8_t last_midi_message[3];
uint32_t no_messages_send_count = 0;

const uint8_t row_pins[MATRIX_ROWS] = {ROW_0_PIN, ROW_1_PIN, ROW_2_PIN, ROW_3_PIN};


typedef enum {
    PRINT_PRESSED,
    DISPLAY_LAST_BUTTON,
    CONTINUOUS_MIDI,
    NUM_DEBUG_MODES
} Debug_Mode;

bool debug_settings[NUM_DEBUG_MODES];


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
    init_segment_display();

    multicore_launch_core1(&core1_main);

    check_debug();

    while(1) {
        key_matrix_task();
        midi_task();
        sleep_ms(1);
	};
};


int main() {
	
	core0_main();

    return 0;
}



void init_gpio() {

    gpio_init_mask(matrix_out_mask | matrix_in_mask);
    gpio_set_function_masked(matrix_out_mask | matrix_in_mask, GPIO_FUNC_SIO);

    gpio_set_dir_in_masked(matrix_in_mask); 

	for (uint pin = 0; pin < 32; ++pin) {
		if (matrix_in_mask & (1u << pin)) {

            // Change to pull down with diodes in correct orientation
			gpio_pull_up(pin);
		}
	}

    gpio_set_dir_out_masked(matrix_out_mask);

    gpio_set_dir(BANK_UP_PIN, GPIO_IN);
    gpio_set_dir(BANK_DOWN_PIN, GPIO_IN); 
    gpio_pull_up(BANK_UP_PIN);
    gpio_pull_up(BANK_DOWN_PIN);

}

void init_segment_display() {

    max7219_init(&display,
                 MAX7219_SPI_PORT,
                 MAX7219_LOAD_PIN,
                 MAX7219_CLK_PIN,
                 MAX7219_MOSI_PIN,
                 MAX7219_NUM_SEGMENTS);

    max7219_set_decode_mode(&display, BCD_DECODE);

    max7219_display_number_2_digits(&display, 0);
};

void init_phisical_midi () {
    // gpio_set_function(MIDI_TX_PIN, GPIO_FUNC_UART);
    // gpio_set_function(MIDI_RX_PIN, GPIO_FUNC_UART);
};

void write_midi(uint8_t const* message, uint8_t lenght) {

    #ifdef USB_MIDI_ENABLE
    tud_midi_n_stream_write(0, 0, message, lenght);
    #endif // USB_MIDI_ENABLE

    #ifdef PHY_MIDI_ENABLE
    // write phy midi
    #endif

}

void check_debug() {

    // also print these as active if set in header
    debug_settings[DISPLAY_LAST_BUTTON] = DEBUG_DISPLAY_LAST_BUTTON;
    debug_settings[PRINT_PRESSED] = DEBUG_PRINT_PRESSED;
    debug_settings[CONTINUOUS_MIDI] = DEBUG_CONTINUOUS_MIDI;

    // read keyboard for debug keys
    key_matrix_task();
    // printf("buttons: %08X \n", (int)buttons_pressed);

    if (buttons_pressed & 1) {
        debug_settings[DISPLAY_LAST_BUTTON] = true;
        printf("[DEBUG] DISPLAY_LAST_BUTTON Active\n");
    };
    if (buttons_pressed & (1 << 1)) {
        debug_settings[PRINT_PRESSED] = true;
        printf("[DEBUG] PRINT_PRESSED Active\n");
    };
    if (buttons_pressed & (1 << 2)) {
        debug_settings[CONTINUOUS_MIDI] = true;
        printf("[DEBUG] CONTINUOUS_MIDI Active\n");
    };
};

void midi_task() {

    uint8_t msg[3];
    uint32_t changed_buttons = buttons_pressed ^ previous_buttons_pressed;

    if (debug_settings[CONTINUOUS_MIDI]) {
        no_messages_send_count++;
        if (no_messages_send_count > 1000) {
            write_midi(last_midi_message, 3);
            no_messages_send_count = 0;
        };
    };
	
    if (debug_settings[PRINT_PRESSED]) {
        printf("buttons: %08X \n", (int)buttons_pressed);
    };

    msg[0] = 0x90 | (current_bank / 4); 

    for (int i = 0; i < 32; i++) {
        uint32_t mask = 1 << i;

        if (changed_buttons & mask) {
            
            msg[1] = i + ((current_bank % 4) * 32); 

            if (buttons_pressed & mask) {
                // Button pressed
                msg[2] = 127; // velocito / on

                if (debug_settings[PRINT_PRESSED]) {
                    printf("Button %d pressed\n", i);
                };

            } else {
                // Button released
                msg[2] = 0; // velocito / off
 
                if (debug_settings[PRINT_PRESSED]) {
                    printf("Button %d released\n", i);
                };               
            }

            write_midi(msg, 3);
            memcpy(last_midi_message, msg, sizeof(msg));
            no_messages_send_count = 0;

        }

    }

    previous_buttons_pressed = buttons_pressed;
}


static bool bank_up_last = true;
static bool bank_down_last = true;

void key_matrix_task() {

    bool bank_up_current = gpio_get(BANK_UP_PIN);
    bool bank_down_current = gpio_get(BANK_DOWN_PIN);

    if (!bank_up_current & bank_up_last) {
        if (current_bank == 63) {
            current_bank = 0;
        } else {
            current_bank++;
        }
        max7219_display_number_2_digits(&display, current_bank);
    }

    if (!bank_down_current & bank_down_last) {
        if (current_bank == 0) {
            current_bank = 63;
        } else {
            current_bank--;
        }
        max7219_display_number_2_digits(&display, current_bank);
    }

    bank_up_last = bank_up_current;
    bank_down_last = bank_down_current;


    // Switch high/low values of gpio read when connecting diodes in correct orientation.
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
