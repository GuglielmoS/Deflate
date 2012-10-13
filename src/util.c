#include "util.h"

/**
 * Shows the first 'bits' bit of 'c'.
 */
void show_bits(uint32_t value, uint8_t bits) 
{
    uint32_t mask = 1 << (bits - 1);

    for (int i = 1; i <= bits; i++) {
        putchar(value & mask ? '1' : '0');
        value <<= 1;
    }
    putchar('\n');
}

/**
 * Returns the n-th bit of 'byte' as an unsigned integer of 8 bit.
 */
uint8_t nth_bit(int8_t byte, int8_t n) {
    return (byte & (0x90 >> (n-1))) ? 1 : 0; 
}

/**
 * Returns the first bit of 'byte' as an unsigned integer of 8 bit.
 */
uint8_t first_bit(int8_t byte) {
    return nth_bit(byte, 1);
}

