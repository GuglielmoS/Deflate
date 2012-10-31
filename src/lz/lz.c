#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lz.h"

/**
 * Returns a stream of LZ_Element that represents the
 * LZ77 encoding of 'message'.
 */
LZ_Stream* LZ_encode_stream(const uint8_t *buffer, size_t buf_size) 
{
    LZ_Stream *out_s = LZ_Stream_new();

    size_t i = 0;
    size_t buf_end = 0;
    while (i < buf_size) {
        size_t length = 0;
        for (size_t j = 0; j < buf_end; j++) {
            if (buffer[j] == buffer[i]) {
                size_t k = 0;
                while (k < (buf_size-i) && k < (buf_size-j) && k < MAX_SEQ_LEN) {
                    if (buffer[j+k] != buffer[i+k]) break;
                    k++;
                }
                length = k;

                if (length >= MIN_SEQ_LEN) {
                    out_s->append(out_s, LZ_Pair_new((LZ_Pair){buf_end-j, length}));
                    i += length;
                    break;
                }
            }
        }

        if (length < MIN_SEQ_LEN) {
            out_s->append(out_s, LZ_Literal_new(buffer[i]));
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
LZ_Stream* LZ_decode_stream(const LZ_Stream *in_s)
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

void LZ_encode_file(FILE *in_f, FILE *out_f)
{
    if (in_f != NULL && out_f != NULL) {
        // retrives the file size
        long file_size = 0;
        fseek(in_f, 0, SEEK_END);
        file_size = ftell(in_f);
        rewind(in_f);

        uint8_t *file_content = (uint8_t*)malloc(sizeof(uint8_t) * file_size);

        fread((uint8_t*)file_content, sizeof(uint8_t), file_size, in_f);

        if (file_content != NULL) {
            LZ_Stream *out_s = LZ_encode_stream(file_content, file_size);

            for (size_t i = 0; i < out_s->get_size(out_s); i++) {
                LZ_Element *cur_el = out_s->get(out_s, i);

                uint8_t type = 0x00;
                if (cur_el->is_literal(cur_el)) {
                    type = 0xFF;
                }

                fwrite((uint8_t*)&type, sizeof(uint8_t), 1, out_f);

                if (type == 0x00) {
                    fwrite((LZ_Pair*)&(cur_el->value.p),sizeof(LZ_Pair),1,out_f);
                }
                else {
                    fwrite((LZ_Literal*)&(cur_el->value.l),sizeof(LZ_Literal),1,out_f);
                }
            }

            fclose(out_f);
            fclose(in_f);
        }
    }
}

void LZ_decode_file(FILE *in_f, FILE *out_f)
{
    if (in_f != NULL && out_f != NULL) {
        // creates a new LZ_Stream
        LZ_Stream *in_s = LZ_Stream_new();

        uint8_t cur_type = 0x00;
        while (fread((uint8_t*)&cur_type, sizeof(uint8_t), 1, in_f) == 1) {
            LZ_Element *cur_el = LZ_Element_new();

            // if is a pair
            if (cur_type == 0x00) {
                LZ_Pair p;
                fread((LZ_Pair*)&p, sizeof(LZ_Pair), 1, in_f);

                cur_el->set_pair_type(cur_el);
                cur_el->set_pair(cur_el, p);
            }
            else {
                LZ_Literal lit;
                fread((LZ_Literal*)&lit, sizeof(LZ_Literal), 1, in_f);

                cur_el->set_literal_type(cur_el);
                cur_el->set_literal(cur_el, lit);
            }

            in_s->append(in_s, cur_el);
        }

        LZ_Stream *out_s = LZ_decode_stream(in_s);

        for (size_t i = 0; i < out_s->get_size(out_s); i++) {
            LZ_Element *cur_el = out_s->get(out_s, i);
            LZ_Literal cur_lit = cur_el->get_literal(cur_el);
            fwrite((LZ_Literal*)&cur_lit, sizeof(LZ_Literal), 1, out_f);
        }

        fclose(out_f);
        fclose(in_f);
    }
}
