#ifndef __BIT_STREAM__
#define __BIT_STREAM__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "util.h"
#include "bit_op.h"
#include "bit_vec.h"

/**
 * Represents a data structure that allows to read and write
 * bit to bit on a valid FILE descriptor.
 */
typedef struct {
    FILE *fd;               // file descriptor of the stream

    Bit_Vec *bits_buf;      // bits buffer

    uint8_t padding_bits;   // number of bits written as padding
                            // at the end of the last write

    size_t max_buf_size;    // maximum size of the temporary buffer in bytes
                            // (that is when will be called the read/write function)

    uint32_t cur_pos;        // current position in the bits buffer (used while reading)

    uint32_t bytes_processed; // number of bytes processed
    uint32_t file_size;       // size of the file in bytes

    bool file_finished; // true if there aren't more data to read
    bool last_chunk;    // true if the current Bit_Vec is referred to the last file's chunk

} Bit_Stream;

/*** USEFUL MACROS ***/
#define BIT_STREAM_IS_FILE_FINISHED(bs) ((bs)->file_finished == true)

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
