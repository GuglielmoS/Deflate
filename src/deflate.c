#include <stdbool.h>
#include <string.h>

#include "list/list.h"
#include "lz/lz_queue.h"
#include "lz/lz_element.h"
#include "hash_table/hash_table.h"
#include "file_stream/file_stream.h"
#include "history_buffer/history_buffer.h"

#define LZ_MIN_SEQ_LEN 5
#define LZ_MAX_SEQ_LEN 258

//#define DEBUG

void LZ_decode_queue(LZ_Queue *queue, FILE *f_out)
{
    if (f_out != NULL) {
        History_Buffer *hbuf   = History_Buffer_new();

        while (!LZ_Queue_is_empty(queue)) {
            LZ_Element *next_el = LZ_Queue_dequeue(queue);

            if (next_el->is_literal(next_el)) {
                History_Buffer_add(hbuf, next_el->get_literal(next_el));

                uint8_t byte = next_el->get_literal(next_el);
                fwrite((uint8_t*)&byte, sizeof(uint8_t), 1, f_out);
            }
            else {
                size_t init_pos = (hbuf->next_pos - next_el->get_distance(next_el));

#ifdef DEBUG
                printf("[D: %d, L: %d]\n",next_el->get_distance(next_el), next_el->get_length(next_el));
                printf("HISTORY BUF:\n");
                for (size_t i = 0; i < hbuf->next_pos; i++) {
                    putchar(History_Buffer_get(hbuf, i));
                }
                putchar('\n');

                printf("STO AGGIUNGENDO: ");
#endif

                for (size_t i = 0; i < next_el->get_length(next_el); i++) {
                    uint8_t byte = History_Buffer_get(hbuf, init_pos + i);
                    History_Buffer_add(hbuf, byte);

#ifdef DEBUG
                    printf("0x%X ", byte);
#endif

                    fwrite((uint8_t*)&byte, sizeof(uint8_t), 1, f_out);
                }

#ifdef DEBUG
                putchar('\n');
                getchar();
#endif
            }
        }
        History_Buffer_destroy(hbuf);
    }
}

void Deflate_encode(const char *in_file_name, const char *out_file_name)
{
    History_Buffer *history_buf = History_Buffer_new();
    File_Stream    *in_fs       = File_Stream_new(in_file_name);
    LZ_Queue        lz_queue;
    Hash_Table      h_table;

    // data structures init
    Hash_Table_init(h_table);
    LZ_Queue_init(&lz_queue);

    uint8_t next3B[3];
    bool    finished = false;
    size_t  i, last_buf_pos, next_byte_start = 0;

    // for statistics
    unsigned long lit_count = 0, pair_count = 0;

    while (!finished) {
        // retrieving the next three bytes
        i = next_byte_start;
        last_buf_pos = history_buf->next_pos - next_byte_start;
        while (!(in_fs->is_finished) && i < 3) {
            next3B[i++] = File_Stream_next_byte(in_fs);
            History_Buffer_add(history_buf, next3B[i-1]);
        }

        // if we are at the end of the file, we could have 2 or
        // less characters, so we put them directly to the LZ_Queue
        // as literals, and set the 'finished' flag to 'true'.
        if (in_fs->is_finished || i < 3) {
            //if (i < 3) i--;
            for (size_t j = 0; j < i; j++) {
                LZ_Queue_enqueue(&lz_queue, LZ_Literal_new(next3B[j]));
                lit_count++;
            }

            finished = true;
        }
        else {
            // in that case we have to process the current 3 bytes
            // so we use the hash table to retrieve a linked list
            // in which we can find the position of them in the history buffer
            List chain = Hash_Table_get(h_table, next3B);

            // if the chain is empty, the first byte is written in output
            // thus the next three bytes sequence will be equal to the last
            // sequence except the third byte that will be read from the file.
            if (chain == NULL) {
                LZ_Queue_enqueue(&lz_queue, LZ_Literal_new(next3B[0]));
                lit_count++;

                // put the sequence in the hash table
                // last_buf_pos represent the position of the current 3 bytes sequence.
                Hash_Table_put(h_table, next3B, last_buf_pos);

                // skip the first byte and proceed with a new sequence
                next3B[0] = next3B[1]; next3B[1] = next3B[2];
                next_byte_start = 2;
            }
            else {
                // max sequence lenght & position
                int    max_seq_length   = 0;
                size_t max_seq_distance = -1;

                // save the current information about the file stream
                // and the history buffer for the future restore
                size_t fs_buf_pos           = in_fs->buf_pos,
                       fs_n_available_bytes = in_fs->n_available_bytes,
                       max_n_avail_bytes    = in_fs->n_available_bytes,
                       max_fs_buf_pos       = in_fs->buf_pos;

                bool   fs_is_finished = in_fs->is_finished;

                size_t history_buf_pos     = history_buf->next_pos,
                       max_history_buf_pos = history_buf->next_pos;

                uint8_t tmp_hist_buf[LZ_MAX_SEQ_LEN];

                // find the longest occurence by trying with every position
                // of the chain
                while (chain != NULL) {
                    // check if the three bytes are those which we are searching for
                    size_t k = 0;
                    size_t buf_start_pos = chain->value;

                    while (k < 3) {
                        uint8_t buf_byte = History_Buffer_get(history_buf,buf_start_pos+k);
                        if (buf_byte != next3B[k]) break;
                        k++;
                    }

                    // if there is a match, find the length of the sequence
                    if (k == 3) {
                        while (in_fs->n_available_bytes > 2 && k < LZ_MAX_SEQ_LEN &&
                               history_buf->next_pos < HISTORY_BUFFER_SIZE-1) {
                            // getting the next byte
                            uint8_t next_byte = File_Stream_next_byte(in_fs);

                            // updating the buffer with the new byte
                            History_Buffer_add(history_buf, next_byte);

                            // retrieving the current byte on the buffer
                            uint8_t buf_byte = History_Buffer_get(history_buf,buf_start_pos+k);

                            // if the new byte from the file isn't equal to
                            // the next byte in the buffer, the sequence is
                            // terminated
                            if (next_byte != buf_byte) break;
                            else                       k++;
                        }

                        if (in_fs->n_available_bytes <= 2) {
                            size_t j = 0;
                            while (j < in_fs->n_available_bytes) {
                                uint8_t next_byte = in_fs->buffer[in_fs->buf_pos+j];
                                History_Buffer_add(history_buf, next_byte);
                                uint8_t buf_byte = History_Buffer_get(history_buf,buf_start_pos+k+j);

                                if (next_byte != buf_byte) break;
                                else                       j++;
                            }
                            in_fs->n_available_bytes -= j+1;
                            in_fs->buf_pos += j+1;
                            k += j;
                        }

                        // if the current sequence is the longest
                        if (k > max_seq_length) {
                            max_seq_length    = k;
                            max_seq_distance  = last_buf_pos - buf_start_pos;
                            max_n_avail_bytes = in_fs->n_available_bytes;
                            max_fs_buf_pos    = in_fs->buf_pos;

                            // save the bytes written in the history buffer
                            // for the next restore
                            memcpy(tmp_hist_buf, history_buf->buf + buf_start_pos + 3, k-3);
                            max_history_buf_pos = history_buf->next_pos;
                        }

                        // restore the information of the file stream and the
                        // history buffer
                        in_fs->buf_pos           = fs_buf_pos;
                        in_fs->n_available_bytes = fs_n_available_bytes;
                        in_fs->is_finished       = fs_is_finished;
                        history_buf->next_pos    = history_buf_pos;
                    }
                    else {
                        // this is not a valid occurrence
                        // because the first 3 bytes are not equal
                        // to the currents that we already have
                    }

                    // proceed to the next position
                    chain = chain->next;
                }

                // if max_seq_pos is equal to -1, it means that the 3 current
                // bytes hash is referred to a list of positions that doesn't
                // contain them. That behaviour is caused by an hash collision.
                if (max_seq_distance == -1 || max_seq_length < LZ_MIN_SEQ_LEN) {
                    LZ_Queue_enqueue(&lz_queue, LZ_Literal_new(next3B[0]));
                    lit_count++;

                    // skip the first byte and proceed with a new sequence
                    next3B[0] = next3B[1]; next3B[1] = next3B[2];
                    next_byte_start = 2;
                }
                else {
                    LZ_Queue_enqueue(&lz_queue,LZ_Pair_new((LZ_Pair){max_seq_distance,max_seq_length}));
                    pair_count++;

                    // restore the information of the file stream and the history buffer
                    in_fs->buf_pos = max_fs_buf_pos - 1;
                    in_fs->n_available_bytes = max_n_avail_bytes + 1;
                    if (in_fs->n_available_bytes <= 0) {
                        File_Stream_force_reload(in_fs);
                    }

                    // restore the history buffer
                    memcpy(history_buf->buf+history_buf_pos, tmp_hist_buf, max_seq_length-3);
                    history_buf->next_pos = (history_buf_pos + max_seq_length - 3) % HISTORY_BUFFER_SIZE;

                    // restore the start position of the next three bytes vector
                    next_byte_start = 0;
                }
            }
        }
    }

    printf("STATS:\n");
    printf("\tLiteral:   %lu\n", lit_count);
    printf("\tD&L pair:  %lu\n", pair_count);
    printf("\tTot Bytes: %lu\n", lit_count + pair_count*3);

    // LZ decompression test
    FILE *out_fp = fopen(out_file_name, "wb");
    LZ_decode_queue(&lz_queue, out_fp);
    fclose(out_fp);

    // freeing memory
    History_Buffer_destroy(history_buf);
    File_Stream_destroy(in_fs);
    LZ_Queue_destroy(&lz_queue);
}

