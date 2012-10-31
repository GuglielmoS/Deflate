#ifndef __LZ__
#define __LZ__

#include <stdio.h>
#include <stdint.h>

#include "lz_stream.h"

#define MAX_BUFFER_SIZE 32768
#define MAX_SEQ_LEN     258
#define MIN_SEQ_LEN     5

LZ_Stream* LZ_encode_stream(const uint8_t *buffer, size_t size);
LZ_Stream* LZ_decode_stream(const LZ_Stream *s);
void       LZ_encode_file(FILE *in_f, FILE *out_f);
void       LZ_decode_file(FILE *in_f, FILE *out_f);

#endif /* __LZ__ */

