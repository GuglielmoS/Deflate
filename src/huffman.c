#include "huffman.h"

uint16_t get_prefix_code(uint16_t edoc)
{
    if      (edoc <= 143) return edoc_init_values[0] + edoc;
    else if (edoc <= 255) return edoc_init_values[1] + edoc - 144;
    else if (edoc <= 279) return edoc_init_values[2] + edoc - 256;
    else                  return edoc_init_values[3] + edoc - 280;
}

uint8_t get_edoc_length(uint16_t edoc)
{
    if      (edoc <= 143) return edoc_length[0];
    else if (edoc <= 255) return edoc_length[1];
    else if (edoc <= 279) return edoc_length[2];
    else                  return edoc_length[3];
}

void Huffman_get_end_block_separator(Bit_Vec *bv)
{
    Bit_Vec_add_n_ls_bits_from_word(bv, 256, 7);
}

void Huffman_get_literal_code(uint8_t literal, Bit_Vec *bv)
{
    Bit_Vec_add_n_ls_bits_from_word(bv, literal, get_edoc_length(literal));
}

void Huffman_get_length_code(uint16_t length, Bit_Vec *bv)
{
    size_t i = 0;
    while (lens[i] < length) i++; // FIND LENGTH POS

    uint16_t edoc = 257 + i;
    if (lens[i] != length) edoc--;

    Bit_Vec_add_n_ls_bits_from_word(bv, get_prefix_code(edoc),get_edoc_length(edoc));
    if (i > 0) {
        Bit_Vec_add_n_ls_bits_from_byte(bv, length-lens[i-1], lext[i-1]);
    }
}

void Huffman_get_distance_code(uint16_t distance, Bit_Vec *bv)
{
    size_t i = 0;
    while (dists[i] < distance) i++; // FIND DISTANCE POS
    if (dists[i] != distance) i--;

    Bit_Vec_add_n_ls_bits_from_byte(bv, i, 5);
    if (i > 0) {
        Bit_Vec_add_n_ls_bits_from_byte(bv, distance-dists[i], dext[i]);
    }
}

/*
Bit_Vec *Huffman_get_end_block_separator()
{
    Bit_Vec *code = Bit_Vec_create();

    Bit_Vec_add_n_ls_bits_from_word(code, 256, 7);

    return code;
}

Bit_Vec *Huffman_get_literal_code(uint8_t literal)
{
    Bit_Vec *code = Bit_Vec_create();

    Bit_Vec_add_n_ls_bits_from_word(code, literal, get_edoc_length(literal));

    return code;
}

Bit_Vec *Huffman_get_length_code(uint16_t length)
{
    size_t i = 0;
    while (lens[i] < length) i++; // FIND LENGTH POS

    uint16_t edoc = 257 + i;
    if (lens[i] != length) edoc--;

    Bit_Vec *code = Bit_Vec_create();
    Bit_Vec_add_n_ls_bits_from_word(code, get_prefix_code(edoc),get_edoc_length(edoc));
    if (i > 0) {
        Bit_Vec_add_n_ls_bits_from_byte(code, length-lens[i-1], lext[i-1]);
    }

    return code;
}

Bit_Vec *Huffman_get_distance_code(uint16_t distance)
{
    size_t i = 0;
    while (dists[i] < distance) i++; // FIND DISTANCE POS
    if (dists[i] != distance) i--;

    Bit_Vec *code = Bit_Vec_create();
    Bit_Vec_add_n_ls_bits_from_byte(code, i, 5);
    if (i > 0) {
        Bit_Vec_add_n_ls_bits_from_byte(code, distance-dists[i], dext[i]);
    }

    return code;
}
*/
