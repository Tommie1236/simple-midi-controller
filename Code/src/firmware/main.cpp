#include "main.h"

// rp2040 
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

// c/cpp 
#include <unordered_map>

// tinyusb
#include "bsp/board.h"
#include "tusb.h"



void midi_task();
void key_matrix_task();
void segment_display_task();
void init_gpio();
void init_segment_display();
void init_phisical_midi();
void check_debug();

uint8_t current_bank = 0;

uint32_t tud_midi_n_stream_write(uint8_t itf, uint8_t cable_num, uint8_t const* buffer, uint32_t bufsize);  // vscode thinks this isn't defined, but is defined at compile and complies correctly. //TODO: remove when fixed or finished

uint32_t buttons_pressed = 0x00000000;
uint32_t previous_buttons_pressed = 0x00000000;

const uint8_t row_pins[MATRIX_ROWS] =
    {
        ROW_0_PIN,
        ROW_1_PIN,
        ROW_2_PIN,
        ROW_3_PIN
    };

const uint8_t collumn_pins[MATRIX_COLS] =
    {
        COL_A_PIN,
        COL_B_PIN,
        COL_C_PIN,
        COL_D_PIN,
        COL_E_PIN,
        COL_F_PIN,
        COL_G_PIN,
        COL_H_PIN
    };

enum class Debug_Mode {
    PRINT_PRESSED = DEBUG_PRINT_PRESSED,
    DISPLAY_LAST_BUTTON = DEBUG_DISPLAY_LAST_BUTTON,
    CONTINUES_MIDI = DEBUG_CONTINUES_MIDI,
    NUM_DEBUG_MODES
}

std::unordered_map<Debug_Mode, bool> debug_settings;


// TODO: change to new debug modes.
#if DEBUG_PRINT
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
    // init_segment_display();

    multicore_launch_core1(&core1_main);

    check_debug();

    while(1) {
        key_matrix_task();
        midi_task();
        sleep_ms(1);
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
    spi_init(MAX7219_SPI_PORT, 10000 * 1000); // 10MHz

    gpio_set_dir(MAX7219_CS_PIN, GPIO_OUT);
    gpio_set_function(MAX7219_CS_PIN, GPIO_FUNC_SIO);
    gpio_set_function(MAX7219_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MAX7219_MOSI_PIN, GPIO_FUNC_SPI);
};

void init_phisical_midi () {
    // gpio_set_function(MIDI_TX_PIN, GPIO_FUNC_UART);
    // gpio_set_function(MIDI_RX_PIN, GPIO_FUNC_UART);
};

void check_debug() {
    // read keyboard for debug keys
    key_matrix_task();
    if (buttons_pressed & 1) {
        debug_settings[Debug_Mode::DISPLAY_LAST_BUTTON] = true;
        printf("[DEBUG] DISPLAY_LAST_BUTTON Active");
    } else if (buttons_pressed & (1 << 1)) {
        debug_settings[Debug_Mode::PRINT_PRESSED] = true;
        printf("[DEBUG] PRINT_PRESSED Active");
    } else if (buttons_pressed & (1 << 2)) {
        debug_settings[Debug_Mode::CONTINUES_MIDI] = true;
        printf("[DEBUG] CONTINUES_MIDI Active");
    };
};

void midi_task() {

    uint8_t msg[3];
    uint32_t changed_buttons = buttons_pressed ^ previous_buttons_pressed;
	
    // debug_printf("buttons: %08X \n", int(buttons_pressed));

    msg[0] = 0x90 | (current_bank / 4); 

    for (int i = 0; i < 32; i++) {
        uint32_t mask = 1 << i;

        if (changed_buttons & mask) {
            
            msg[1] = i + ((current_bank % 4) * 32); // Note number TODO: apply bank
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
    }

    if (!bank_down_current & bank_down_last) {
        if (current_bank == 0) {
            current_bank = 63;
        } else {
            current_bank--;
        }
    }

    bank_up_last = bank_up_current;
    bank_down_last = bank_down_current;


    buttons_pressed = 0;

    for ( int row_idx = 0; row_idx < MATRIX_ROWS; ++row_idx) {
        
        gpio_set_mask(matrix_out_mask);

        gpio_put(row_pins[row_idx], 0);
        sleep_us(50);

        uint32_t gpio_state = gpio_get_all();

        for (int collumn = 0; collumn < MATRIX_COLS; ++collumn) {
            uint8_t pin = collumn_pins[collumn];
        }
    }
}
