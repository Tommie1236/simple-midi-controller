#ifndef SEGMENTS_H
#define SEGMENTS_H

#include <unordered_map>
#include <pico/stdlib.h>

const std::unordered_map<char, uint8_t> SEGMENTS = {
//  Char,   GFEDCBA
    {'0', 0b0111111},
    {'1', 0b0000110},
    {'2', 0b1011011},
    {'3', 0b1001111},
    {'4', 0b1100110},
    {'5', 0b1101101},
    {'6', 0b1111101},
    {'7', 0b0000111},
    {'8', 0b1111111},
    {'9', 0b1101111},
    {'a', 0b1110111},
    {'b', 0b1111100},
    {'c', 0b0111001},
    {'d', 0b1011110},
    {'e', 0b1111001},
    {'f', 0b1110001},
    {'g', 0b0111101},
    {'h', 0b1110100},
    {'i', 0b0000100},
    {'j', 0b0011110},
    {'l', 0b0111000},
    {'n', 0b0110111},
    {'o', 0b1011100},
    {'p', 0b1110011},
    {'q', 0b1100111},
    {'r', 0b1010000},
    {'s', 0b1101101},
    {'t', 0b1111000},
    {'u', 0b0011100},
    {'x', 0b1110110},
    {'y', 0b1101110},
    {'-', 0b1000000},
    {' ', 0b0000000}
};

#endif // SEGMENTS_H


// Some characters are not possible with a 7 segment display.
// The below are not possible:
// k, m, v, w, z
// Some are possible but are the same as others:
// v is the same as u.
// z is the same as 2.
// And the letters k, m and w are just not possible with a 7 segment display.
// If you know a better "font" or can inplement more characters and keep everyting readable please let me know.