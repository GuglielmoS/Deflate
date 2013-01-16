#ifndef __BIT_VEC__
#define __BIT_VEC__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "util.h"
#include "bit_op.h"

/**
 * Represents the data structure used for dealing with a vector
 * of bits.
 */
typedef struct {
    uint8_t *buf;  // bits buffer
    size_t   cur_size; // current number of bits stored
    size_t   max_size; // maximum number of bits
} Bit_Vec;

// 1 byte of default buffer size
#define BIT_VEC_INIT_BUF_SIZE 1

// useful macros
#define BIT_VEC_SIZE(bv)      ((bv)->cur_size)
#define BIT_VEC_IS_FULL(bv)   ((bv)->cur_size == (bv)->max_size)
#define BIT_VEC_GET_BIT(bv,p) (BYTE_BIT_GET((bv)->buf[(p)/8],7-(p)%8))

// create
Bit_Vec* Bit_Vec_create();
Bit_Vec* Bit_Vec_create_with_size(size_t init_size);
Bit_Vec* Bit_Vec_create_with_byte(uint8_t byte);
Bit_Vec* Bit_Vec_create_with_word(uint16_t word);

// add
void Bit_Vec_add_bit(Bit_Vec *bv, uint8_t bit);
void Bit_Vec_add_byte(Bit_Vec *bv, uint8_t byte);
void Bit_Vec_add_n_ls_bits_from_byte(Bit_Vec *bv, uint16_t byte, uint8_t n_bits);
void Bit_Vec_add_bytes(Bit_Vec *bv, uint8_t *bytes, size_t size);
void Bit_Vec_add_word(Bit_Vec *bv, uint16_t word);
void Bit_Vec_add_n_ls_bits_from_word(Bit_Vec *bv, uint16_t word, uint8_t n_bits);

// get
uint8_t Bit_Vec_get_bit(Bit_Vec *bv, size_t bit_pos);
size_t Bit_Vec_size(Bit_Vec *bv);

// util
Bit_Vec* Bit_Vec_dup(Bit_Vec *bv);
Bit_Vec* Bit_Vec_concat(Bit_Vec *bv1, Bit_Vec *bv2);

// destroy
void Bit_Vec_destroy(Bit_Vec *bv);

#endif /* __BIT_VEC__ */
