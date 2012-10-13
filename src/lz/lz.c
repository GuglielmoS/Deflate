#include <stdio.h>
#include <string.h>

#include "lz.h"

/**
 * Returns a stream of LZ_Element that represents the
 * LZ77 encoding of 'message'.
 */
LZ_Stream* LZ_encode(const char *buffer) 
{
    LZ_Stream *out_s = LZ_Stream_new();

    size_t i = 0;
    size_t buf_end = 0;
    size_t buffer_length = strlen(buffer);

    while (i < buffer_length) {

        size_t length = 0;
        for (size_t j = 0; j < buf_end; j++) {

            if (buffer[j] == buffer[i]) {
                
                size_t k = 0;
                while (k < buffer_length) {
                    if (buffer[j+k] == buffer[i+k]) length++;
                    else break;
                    k++;
                }
                
                if (length > 0) {
                    out_s->append(out_s, 
                                  LZ_Pair_new((LZ_Pair){buf_end-j, length}));
                    i += length;
                    break;
                }
            }
        }
        
        if (length == 0) {
            out_s->append(out_s,
                          LZ_Literal_new((LZ_Literal)buffer[i]));
            i++;
        }

        buf_end++;
    }

    return out_s;
}

/**
 * Returns a stream of LZ_Element (in particular LZ_LITERAL) by decoding the 
 * LZ_Stream 'stream'.
 */
LZ_Stream* LZ_decode(const LZ_Stream *in_s)
{
    LZ_Stream *out_s = LZ_Stream_new();

    size_t stream_size = in_s->get_size(in_s);
    for (size_t i = 0; i < stream_size; i++) {
        LZ_Element *cur_e = in_s->get(in_s, i);

        // if it's a literal
        if (cur_e->is_literal(cur_e)) {
            out_s->append(out_s, cur_e);
        }
        else {
            // steps back in the buffer of 'distance' position 
            // and gets the following 'length' literal.
            size_t distance  = cur_e->get_distance(cur_e),
                   length    = cur_e->get_length(cur_e),
                   start_pos = i - distance,
                   end_pos   = start_pos + length;

            for (size_t j = start_pos; j < end_pos; j++) {
                out_s->append(out_s, out_s->get(out_s, j));
            }
        }
    }

    return out_s;
}

