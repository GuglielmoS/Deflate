#include "bit_vec.h"

/**
 * Allocates and returns a Bit_Vec.
 */
Bit_Vec* Bit_Vec_create()
{
    Bit_Vec *bv = (Bit_Vec*)malloc(sizeof(Bit_Vec));

    if (bv == NULL) {
        die_error("[ERROR-Bit_Vec_create] malloc (1st) failed!\n");
    }

    bv->buf = (uint8_t*)malloc(BIT_VEC_INIT_BUF_SIZE);
    if (bv->buf == NULL) {
        die_error("[ERROR-Bit_Vec_create] malloc (2nd) failed!\n");
    }

    bv->max_size = BIT_VEC_INIT_BUF_SIZE*8;
    bv->cur_size = 0;

    return bv;
}

/**
 * Allocates and returns a Bit_Vec which has the maximum
 * size setted to init_size.
 * 'init_size' must be expressed in bytes.
 */
Bit_Vec* Bit_Vec_create_with_size(size_t init_size)
{
    Bit_Vec *bv = (Bit_Vec*)malloc(sizeof(Bit_Vec));

    if (bv == NULL) {
        die_error("[ERROR-Bit_Vec_create_with_size] malloc (1st) failed!\n");
    }

    bv->buf = (uint8_t*)malloc(init_size);
    if (bv->buf == NULL) {
        die_error("[ERROR-Bit_Vec_create_with_size] malloc (2nd) failed!\n");
    }

    bv->max_size = init_size*8;
    bv->cur_size = 0;

    return bv;
}

/**
 * Creates a Bit_Vec from a byte value (8 bit).
 */
Bit_Vec* Bit_Vec_create_from_byte(uint8_t byte)
{
    Bit_Vec *bv = Bit_Vec_create_with_size(1);
    bv->buf[0] = byte;
    bv->cur_size = 8;

    return bv;
}

/**
 * Creates a Bit_Vec from a word value (16 bit).
 */
Bit_Vec* Bit_Vec_create_from_word(uint16_t word)
{
    Bit_Vec *bv = Bit_Vec_create_with_size(2);
    memcpy(bv->buf, &word, 2);
    bv->cur_size = 16;

    return bv;
}

/**
 * Returns a duplicate of the Bit_Vec 'bv'.
 */
Bit_Vec* Bit_Vec_dup(Bit_Vec *bv)
{
    Bit_Vec *bv2 = (Bit_Vec*)malloc(sizeof(Bit_Vec));

    if (bv2 == NULL) {
        die_error("[ERROR-Bit_Vec_create_with_size] malloc (1st) failed!\n");
    }

    bv2->buf = (uint8_t*)malloc(bv->max_size/8);
    if (bv2->buf == NULL) {
        die_error("[ERROR-Bit_Vec_create_with_size] malloc (2nd) failed!\n");
    }

    memcpy(bv2->buf,bv->buf,bv->max_size/8);
    bv2->max_size = bv->max_size;
    bv2->cur_size = bv->cur_size;

    return bv2;
}

/**
 * Reallocates the buffer used for storing bits.
 */
void Bit_Vec_grow_store(Bit_Vec *bv)
{
    bv->buf = (uint8_t*)realloc(bv->buf, bv->max_size/4);
    if (bv->buf == NULL) {
        die_error("[ERROR-Bit_Vec_grow_store] realloc failed!\n");
    }

    bv->max_size *= 2;
}

/*+
 * Adds a bit to the buffer.
 */
void Bit_Vec_add_bit(Bit_Vec *bv, uint8_t bit)
{
    if (BIT_VEC_IS_FULL(bv)) {
        Bit_Vec_grow_store(bv);
    }

    size_t byte_pos = bv->cur_size / 8,
           bit_pos  = 7 - bv->cur_size % 8;

    bv->buf[byte_pos] = (bit) ?
        BYTE_BIT_SET(bv->buf[byte_pos],bit_pos) :
        BYTE_BIT_CLEAR(bv->buf[byte_pos],bit_pos);

    bv->cur_size++;
}

/**
 * Adds the byte 'byte' to the Bit_Vec 'bv'.
 */
void Bit_Vec_add_byte(Bit_Vec *bv, uint8_t byte)
{
    for (int i = 7; i >= 0; i--) {
        Bit_Vec_add_bit(bv, BYTE_BIT_GET(byte,i));
    }
}

/**
 * Adds the least significant 'n_bits' bits from the byte 'byte' to the Bit_Vec 'bv'.
 */
void Bit_Vec_add_n_ls_bits_from_byte(Bit_Vec *bv, uint16_t byte, uint8_t n_bits)
{
    for (int i = n_bits-1; i >= 0; i--) {
        Bit_Vec_add_bit(bv, BYTE_BIT_GET(byte,i));
    }
}

/**
 * Adds 'size' bytes to the Bit_Vec 'bv'.
 */
void Bit_Vec_add_bytes(Bit_Vec *bv, uint8_t *bytes, size_t size)
{
    if (bv->cur_size % 8 == 0 &&
        bv->max_size - bv->cur_size >= size) {

        memcpy(bv->buf + (bv->cur_size/8), bytes, size);
        bv->cur_size += size*8;
    }
    else {
        for (size_t i = 0; i < size; i++) {
            Bit_Vec_add_byte(bv, bytes[i]);
        }
    }
}

/**
 * Adds the word 'word' to the Bit_Vec 'bv'.
 */
void Bit_Vec_add_word(Bit_Vec *bv, uint16_t word)
{
    for (int i = 15; i >= 0; i--) {
        Bit_Vec_add_bit(bv, WORD_BIT_GET(word,i));
    }
}

/**
 * Adds the least significant 'n_bits' bits from the word 'word' to the Bit_Vec 'bv'.
 */
void Bit_Vec_add_n_ls_bits_from_word(Bit_Vec *bv, uint16_t word, uint8_t n_bits)
{
    for (int i = n_bits-1; i >= 0; i--) {
        Bit_Vec_add_bit(bv, WORD_BIT_GET(word,i));
    }
}

/**
 * Returns the bit at the position 'bit_pos'.
 */
uint8_t Bit_Vec_get_bit(Bit_Vec *bv, size_t bit_pos)
{
    return BYTE_BIT_GET(bv->buf[bit_pos/8],7-bit_pos%8);
}

/**
 * Returns a new Bit_Vec which contains the two Bit_Vec.
 */
Bit_Vec* Bit_Vec_concat(Bit_Vec *bv1, Bit_Vec *bv2)
{
    Bit_Vec *new_bv = Bit_Vec_dup(bv1);

    for (size_t i = 0; i < bv2->cur_size; i++) {
        Bit_Vec_add_bit(new_bv, Bit_Vec_get_bit(bv2, i));
    }

    return new_bv;
}

/**
 * Returns the number of bits stored.
 */
size_t Bit_Vec_size(Bit_Vec *bv)
{
    return bv->cur_size;
}

/**
 * Frees the memory allocated for storing bits.
 */
void Bit_Vec_destroy(Bit_Vec *bv)
{
    if (bv != NULL) {
        bv->cur_size = 0;
        free(bv->buf);
    }
}
