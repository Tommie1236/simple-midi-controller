
// Config made for a RP2040 Zero. (Pi pico in smaller form factor)
// For full pin functions see:
// https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
// (Section 1.4.3)
//      
//               ┏━━━━━┓              
//           ┏━━━┫     ┣━━━┓          
//        5V ┃◎  ┗━━━━━┛  ◎┃ GP0  row 3
//       Gnd ┃■           ◎┃ GP1  row 2
//       3V3 ┃◎           ◎┃ GP2  row 1
// rx   GP29 ┃◎           ◎┃ GP3  row 0
// tx   GP28 ┃◎           ◎┃ GP4  col A
// tx   GP27 ┃◎     □     ◎┃ GP5  col B
// sck  GP26 ┃◎     ┗GP16 ◎┃ GP6  col C
// cs   GP15 ┃◎           ◎┃ GP7  col D
//      GP14 ┃◎ ◎ ◎ ◎ ◎ ◎ ◎┃ GP8  col E
//           ┗━━━━━━━━━━━━━┛
//              ┃ ┃ ┃ ┃ ┗ GP9     col F
//              ┃ ┃ ┃ ┗ GP10      col G
//              ┃ ┃ ┗ GP11        col H
//              ┃ ┗ GP12          bank up
//              ┗ GP13            bank down
//
//  GP17-25 Are available via pads on the bottom side of the board.


#ifndef MAIN_H
#define MAIN_H

#include "pico/stdlib.h"

// Debug Modes, setting here turns the mode persistently on.
// Pressing the according key on boot turn debug mode on for that session.
// ONLY MEANT FOR EASIER DEVELOPMENT, Don't turn on in production compile
#define DEBUG_PRINT_PRESSED         0
#define DEBUG_DISPLAY_LAST_BUTTON   0
#define DEBUG_CONTINUOUS_MIDI       0

// Please don't change. this really simplified the code.
// TODO: may support remapping later.
#define MATRIX_ROWS                 4
#define MATRIX_COLS                 8

#define ROW_0_PIN                   0
#define ROW_1_PIN                   1
#define ROW_2_PIN                   2
#define ROW_3_PIN                   3

#define COL_A_PIN                   4
#define COL_B_PIN                   5
#define COL_C_PIN                   6
#define COL_D_PIN                   7
#define COL_E_PIN                   8
#define COL_F_PIN                   9
#define COL_G_PIN                   10
#define COL_H_PIN                   11

#define BANK_UP_PIN                 12
#define BANK_DOWN_PIN               13


#define MAX7219_NUM_SEGMENTS        2
#define MAX7219_SPI_PORT            spi1

#define MAX7219_LOAD_PIN              15
#define MAX7219_CLK_PIN             26
#define MAX7219_MOSI_PIN            27

// phiysical midi. maybe support later
// #define MIDI_TX_PIN              28
// #define MIDI_RX_PIN              29


const uint32_t matrix_out_mask =  (1 << ROW_0_PIN) | 
                                 (1 << ROW_1_PIN) | 
                                 (1 << ROW_2_PIN) | 
                                 (1 << ROW_3_PIN);

const uint32_t matrix_in_mask = (1 << COL_A_PIN) |
                                 (1 << COL_B_PIN) |
                                 (1 << COL_C_PIN) |
                                 (1 << COL_D_PIN) |
                                 (1 << COL_E_PIN) |
                                 (1 << COL_F_PIN) |
                                 (1 << COL_G_PIN) |
                                 (1 << COL_H_PIN);




#endif // MAIN_H
