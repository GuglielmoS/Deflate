#include "huffman.h"

/**
 * Returns the value reltive to the edoc given as argument.
 */
uint16_t Huffman_get_prefix_code(uint16_t edoc)
{
    if      (edoc <= 143) return edoc_init_values[0] +  edoc;
    else if (edoc <= 255) return edoc_init_values[1] + (edoc - 144);
    else if (edoc <= 279) return edoc_init_values[2] + (edoc - 256);
    else                  return edoc_init_values[3] + (edoc - 280);
}

/**
 * Returns the length in bits of the value relative to the edoc 'edoc'.
 */
uint8_t Huffman_get_edoc_length(uint16_t edoc)
{
    if      (edoc <= 143) return edoc_length[0];
    else if (edoc <= 255) return edoc_length[1];
    else if (edoc <= 279) return edoc_length[2];
    else                  return edoc_length[3];
}

/**
 * Stores in the bits vector 'bv' the huffman code relative to
 * the block separator.
 */
void Huffman_get_end_block_separator(Bit_Vec *bv)
{
    Bit_Vec_add_n_ls_bits_from_byte(bv, 0x00, 7);
}

/**
 * Stores in the bits vector 'bv' the huffman code relative to
 * byte 'literal'.
 */
void Huffman_get_literal_code(uint8_t literal, Bit_Vec *bv)
{
    Bit_Vec_add_n_ls_bits_from_word(bv, Huffman_get_prefix_code(literal), Huffman_get_edoc_length(literal));
}

/**
 * Stores in the bits vector 'bv' the huffman code relative to the length
 * 'length'.
 */
void Huffman_get_length_code(uint16_t length, Bit_Vec *bv)
{
    int i = 0;
    while (lens[i] < length && i < 29) i++; // FIND LENGTH POS
    if (lens[i] != length) i--;

    // prefix code bits
    Bit_Vec_add_n_ls_bits_from_word(bv, Huffman_get_prefix_code(257 + i), Huffman_get_edoc_length(257 + i));
    // extra bits
    Bit_Vec_add_n_ls_bits_from_word(bv, length - lens[i], lext[i]);
}

/**
 * Stores in the bits vector 'bv' the huffman code relative to the distance
 * 'distance'.
 */
void Huffman_get_distance_code(uint16_t distance, Bit_Vec *bv)
{
    int i = 0;
    while (dists[i] < distance && i < 30) i++; // FIND DISTANCE POS
    if (dists[i] != distance) i--;

    // 5 bits fixed-prefix code
    Bit_Vec_add_n_ls_bits_from_word(bv, i, 5);
    // extra bits
    Bit_Vec_add_n_ls_bits_from_word(bv, distance - dists[i], dext[i]);
}

