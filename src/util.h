#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef __UTIL__
#define __UTIL__

void    show_bits(uint32_t value, uint8_t bits);
uint8_t nth_bit(int8_t byte, int8_t n);
uint8_t first_bit(int8_t byte);

#endif /* __UTIL__ */
