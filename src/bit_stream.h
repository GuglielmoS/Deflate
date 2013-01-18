#ifndef __BIT_STREAM__
#define __BIT_STREAM__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "util.h"
#include "bit_op.h"
#include "bit_vec.h"


/**
 * Represents a data structure which allows to read/write
 * in a bit-oriented way a valid FILE descriptor.
 */
typedef struct {
    // file descriptor of the stream
    FILE *fd;

    // bits buffer
    Bit_Vec *bits_buf;

    // number of bits written as padding at the end of the last write call
    uint8_t padding_bits;

    // maximum size of the temporary buffer in bytes
    // (that is when will be called the read/write function)
    uint32_t max_buf_size;

    // current position in the bits buffer (used while reading)
    uint32_t cur_pos;

    // number of bytes processed
    uint32_t processed_bytes;

    // size of the file in bytes
    uint32_t file_size;

    // true if there aren't more data to read
    bool file_finished;

    // true if the current Bit_Vec is referred to the last file's chunk
    bool last_chunk;
} Bit_Stream;


/*** USEFUL MACROS ***/

// adds the bits vector 'bv' to the bits stream 'bs'
#define BIT_STREAM_ADD_BIT_VEC(bs,bv) for (int i=0;i<(bv)->cur_size;i++){Bit_Stream_add_bit(bs_out, BIT_VEC_GET_BIT(bv,i));}

// initialization
void Bit_Stream_init(Bit_Stream *bs, const char *file_name, const char *mode, size_t max_size);
void Bit_Stream_read_n_padding_bits(Bit_Stream *bs);

// add (writing)
void Bit_Stream_add_bit(Bit_Stream *bs, uint8_t value);
void Bit_Stream_add_byte(Bit_Stream *bs, uint8_t byte);
void Bit_Stream_add_word(Bit_Stream *bs, uint16_t word);
void Bit_Stream_add_n_bit(Bit_Stream *bs, Bit_Vec *bv);

// get (reading)
uint8_t Bit_Stream_get_bit(Bit_Stream *bs);
uint8_t Bit_Stream_get_byte(Bit_Stream *bs);
uint16_t Bit_Stream_get_word(Bit_Stream *bs);
Bit_Vec* Bit_Stream_get_n_bit(Bit_Stream *bs, size_t n);

// read
void Bit_Stream_read_next_chunk(Bit_Stream *bs);

// reset
void Bit_Stream_buffer_reset(Bit_Stream *bs);

// write
void Bit_Stream_force_write(Bit_Stream *bs);
void Bit_Stream_write_padding_bits(Bit_Stream *bs);

// close
void Bit_Stream_close(Bit_Stream *bs);
void Bit_Stream_destroy(Bit_Stream *bs);

// util
bool Bit_Stream_finished(Bit_Stream *bs);


#endif /* __BIT_STREAM__ */
