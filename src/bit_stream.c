#include "bit_stream.h"

/**
 * Initializes the Bit_Stream 'bs' with the given values.
 */
void Bit_Stream_init(Bit_Stream *bs, const char *file_name, const char *mode, size_t max_size)
{
    bs->fd = fopen(file_name, mode);
    if (bs->fd == NULL) {
        die_error("[ERROR-Bit_Stream_init] fopen failed!\n");
    }

    bs->bits_buf = Bit_Vec_create_with_size(max_size);
    bs->max_buf_size = max_size;

    // read parameters
    bs->cur_pos = 0;
    bs->file_finished = false;
    bs->last_chunk = false;

    // padding bits
    if (strcmp(mode, "rb") == 0) {
        bs->processed_bytes = 0;
        Bit_Stream_read_n_padding_bits(bs);
        //Bit_Stream_read_next_chunk(bs);
    }
    else {
        bs->padding_bits = 0;
    }
}

/**
 * Reads the number of padding bits at the end of the file and stores it
 * in the Bit_Stream data structure; it stores the file size too.
 * INFO: the number of padding bits is stored as the last byte.
 * Thus the byte before the latter will eventually contains the last bits 
 * plus the padding bits.
 */
void Bit_Stream_read_n_padding_bits(Bit_Stream *bs)
{
    if (bs->fd != NULL) {
        fseek(bs->fd, 1, SEEK_END);
        fread((uint8_t*)&(bs->padding_bits), sizeof(uint8_t), 1, bs->fd);
        bs->file_size = ftell(bs->fd) - 1;
        rewind(bs->fd);
    }
}

/**
 * Adds a bit to the write buffer.
 * If the size of the buffer is equal to the max_buf_size
 * parameter, it writes the whole bits_buffer on the file.
 */
void Bit_Stream_add_bit(Bit_Stream *bs, uint8_t value)
{
    Bit_Vec_add_bit(bs->bits_buf, value);

    if (BIT_VEC_SIZE(bs->bits_buf) >= bs->max_buf_size*8 &&
        BIT_VEC_SIZE(bs->bits_buf) % 8 == 0) {
        Bit_Stream_force_write(bs);
    }
}

/**
 * Adds a byte on the write buffer.
 * If the size of the buffer is equal to the max_buf_size
 * parameter, it writes the whole bits_buffer on the file.
 */
void Bit_Stream_add_byte(Bit_Stream *bs, uint8_t byte)
{
    Bit_Vec_add_byte(bs->bits_buf, byte);

    if (BIT_VEC_SIZE(bs->bits_buf) >= bs->max_buf_size*8 &&
        BIT_VEC_SIZE(bs->bits_buf) % 8 == 0) {
        Bit_Stream_force_write(bs);
    }
}

/**
 * Adds a word (16 bit) on the write buffer.
 * If the size of the buffer is equal to the max_buf_size
 * parameter, it writes the whole bits_buffer on the file.
 */
void Bit_Stream_add_word(Bit_Stream *bs, uint16_t word)
{
    Bit_Vec_add_word(bs->bits_buf, word);

    if (BIT_VEC_SIZE(bs->bits_buf) >= bs->max_buf_size*8 &&
        BIT_VEC_SIZE(bs->bits_buf) % 8 == 0) {
        Bit_Stream_force_write(bs);
    }
}

/**
 * Adds the Bit_Vec 'bv' ot the Bit_Stream 'bs'.
 */
void Bit_Stream_add_n_bit(Bit_Stream *bs, Bit_Vec *bv)
{
    for (size_t i = 0; i < BIT_VEC_SIZE(bv); i++) {
        Bit_Stream_add_bit(bs, Bit_Vec_get_bit(bv, i));
    }
}

/**
 * Writes the bits buffer on the file and sets the field
 * remained_bits to the number of bits added as padding at the end
 * of the file.
 */
void Bit_Stream_force_write(Bit_Stream *bs)
{
    uint32_t n_bits  = bs->bits_buf->cur_size,
             n_bytes = bs->bits_buf->cur_size / 8,
             diff    = n_bytes * 8 - n_bits;

    fwrite((uint8_t*)(bs->bits_buf->buf), sizeof(uint8_t), n_bytes, bs->fd);

    // if it isn't the right number of byte
    if (diff != 0) {
        uint8_t last_byte = 0x00;
        for (int i = 7; i >= 7-diff; i--) {
            if (BYTE_BIT_GET(bs->bits_buf->buf[n_bytes], i)) {
                last_byte = BYTE_BIT_SET(last_byte, i);
            }
        }
        bs->padding_bits = 8 - diff;

        fwrite((uint8_t*)&last_byte, sizeof(uint8_t), 1, bs->fd);
    }

    Bit_Stream_buffer_reset(bs);
}

/**
 * Resets the bits buffer.
 */
void Bit_Stream_buffer_reset(Bit_Stream *bs)
{
    Bit_Vec_destroy(bs->bits_buf);
    free(bs->bits_buf);

    bs->bits_buf = Bit_Vec_create();
}

/**
 * Writes a byte which indicates the number of bits used as padding for
 * completing the write process.
 */
void Bit_Stream_write_padding_bits(Bit_Stream *bs)
{
    fwrite((uint8_t*)&(bs->padding_bits), sizeof(uint8_t), 1, bs->fd);
}

/**
 * Reads the next chunk of data from the file and appends it
 * to the bits buffer.
 */
void Bit_Stream_read_next_chunk(Bit_Stream *bs)
{
    if (bs->bits_buf) {
        Bit_Vec_destroy(bs->bits_buf);
    }

    bs->bits_buf = Bit_Vec_create_with_size(bs->max_buf_size);
    bs->cur_pos = 0;

    uint8_t *data = (uint8_t*)malloc(sizeof(uint8_t) * bs->max_buf_size);
    if (data == NULL) {
        die_error("[ERROR-Bit_Stream_read_next_chunk] malloc failed!\n");
    }

    size_t read_bytes = fread((uint8_t*)data, sizeof(uint8_t), bs->max_buf_size, bs->fd);

    if (read_bytes == 0) {
        bs->file_finished = true;
    }
    else {
        bs->processed_bytes += read_bytes;

        // in that case we have to add all but the last byte which contains
        // the number of padding bits used
        if (bs->processed_bytes == bs->file_size) {
            if (read_bytes > 2) {
                memcpy(bs->bits_buf->buf, data, read_bytes-2);
                bs->bits_buf->cur_size = (read_bytes-2)*8;
            }

            // add the last bits
            for (uint8_t i = 0; i < bs->padding_bits; i++) {
                Bit_Vec_add_bit(bs->bits_buf, BYTE_BIT_GET(data[read_bytes-2], 7-i));
            }

            bs->last_chunk = true;
        }
        else {
            memcpy(bs->bits_buf->buf, data, read_bytes);
            bs->bits_buf->cur_size = read_bytes*8;
        }
    }

    free(data);
}

/**
 * Returns the next bit of the file.
 */
uint8_t Bit_Stream_get_bit(Bit_Stream *bs)
{
    if ((bs->cur_pos / 8 >= bs->max_buf_size &&
         bs->cur_pos % 8 == 0) ||
        (bs->cur_pos == bs->bits_buf->cur_size)) {

        Bit_Stream_read_next_chunk(bs);
    }

    if (bs->file_finished) {
        die_error("[ERROR-Bit_Stream_get_bit] the file is finished, you can't read another bit!\n");
        return 0x00;
    }
    else {
        if (bs->last_chunk && bs->cur_pos == bs->bits_buf->cur_size-1) {
            bs->file_finished = true;
        }

        bs->cur_pos++;
        return Bit_Vec_get_bit(bs->bits_buf, bs->cur_pos-1);
    }
}

/**
 * Returns the next byte of the file.
 * If it is used when there are less than 8 bits available, the
 * byte returned will be composed of the real bits setted according to
 * the file, while the other bits will be setted to zero.
 */
uint8_t Bit_Stream_get_byte(Bit_Stream *bs)
{
    uint8_t byte = 0x00;
    for (size_t i = 0; i < 8; i++) {
        if (Bit_Stream_get_bit(bs)) {
            byte = BYTE_BIT_SET(byte,7-i);
        }
    }

    return byte;
}

/**
 * Returns the next word of the file.
 */
uint16_t Bit_Stream_get_word(Bit_Stream *bs)
{
    return ((Bit_Stream_get_byte(bs) & 0x00FF) << 8) | (Bit_Stream_get_byte(bs) & 0x00FF);
}

/**
 * Returns a Bit_Vec which contains the next 'n' bits of the file.
 */
Bit_Vec* Bit_Stream_get_n_bit(Bit_Stream *bs, size_t n)
{
    Bit_Vec *bv = Bit_Vec_create_with_size(n/8 + ((n % 8) ? 1 : 0));

    for (size_t i = 0; i < n; i++) {
        Bit_Vec_add_bit(bv, Bit_Stream_get_bit(bs));
    }

    return bv;
}

/**
 * Closes the file used as Bit_Stream.
 */
void Bit_Stream_close(Bit_Stream *bs)
{
    fclose(bs->fd);
}

/**
 * Returns true if the file processed is finished; else otherwise.
 */
bool Bit_Stream_finished(Bit_Stream *bs)
{
    return bs->file_finished ||
           (bs->cur_pos == bs->bits_buf->cur_size && bs->last_chunk);
}

void Bit_Stream_destroy(Bit_Stream *bs)
{
    if (bs->fd) {
        fclose(bs->fd);
    }

    if (bs->bits_buf) {
        Bit_Vec_destroy(bs->bits_buf);
        free(bs->bits_buf);
    }
}
