#ifndef __HUFFMAN__
#define __HUFFMAN__

#include "bit_vec.h"

// length offsets
static const uint16_t lens[29] = {
    3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
    35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258};

// length extra bits
static const uint8_t lext[29] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
    3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};

// distance offsets
static const uint16_t dists[30] = {
    1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
    257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
    8193, 12289, 16385, 24577};

// distance extra bits
static const uint8_t dext[30] = {
    0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
    7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};

static const uint16_t edoc_init_values[4] = {
    48,400,0,192};

// edoc values length
static const uint8_t edoc_length[4] = {
    8, 9, 7, 8};

void Huffman_get_end_block_separator(Bit_Vec *bv);
void Huffman_get_literal_code(uint8_t literal, Bit_Vec *bv);
void Huffman_get_length_code(uint16_t length, Bit_Vec *bv);
void Huffman_get_distance_code(uint16_t distance, Bit_Vec *bv);

#endif /* __HUFFMAN__ */