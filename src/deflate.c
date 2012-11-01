#include <stdbool.h>

#include "list/list.h"
#include "hash_table/hash_table.h"
#include "file_stream/file_stream.h"
#include "history_buffer/history_buffer.h"

void DF_encode(const char *in_file_name, const char *out_file_name)
{
    History_Buffer *history_buf = History_Buffer_new();
    File_Stream *in_fs = File_Stream_new(in_file_name);
    Hash_Table h_table;

    // set all to NULL
    Hash_Table_init(h_table);

    uint8_t next3B[3];
    bool    finished = false;
    size_t  i, last_buf_pos,
            next_byte_start = 0;
    while (!finished) {
        // retrieving the next three bytes
        i = next_byte_start;
        last_buf_pos = history_buf->next_pos - next_byte_start;
        while (!(in_fs->is_finished) && i < 3) {
            next3B[i++] = File_Stream_next_byte(in_fs);
            History_Buffer_add(history_buf, next3B[i-1]);
        }

        //printf("\nCUR: %c%c%c\n", next3B[0], next3B[1],  next3B[2]);

        // if we are at the end of the file, we could have 2 or
        // less characters, so we put them directly to the LZ_Queue
        // as literals, and set the 'finished' flag to 'true'.
        if (in_fs->is_finished || i < 3) {
            for (size_t j = 0; j < i-1; j++) {
                printf("%c", next3B[j]);
            }

            finished = true;
        }
        else {
            // in that case we have to process the current 3 bytes
            // so we use the hash table for retrieving a linked list
            // in which we can find the position of them in the history buffer

            List chain = Hash_Table_get(h_table, next3B);
            if (chain == NULL) {
                printf("%c", next3B[0]);
                // put the sequence in the hash table
                // last_buf_pos represent the position of the current
                // 3 bytes sequence.
                Hash_Table_put(h_table, next3B, last_buf_pos);

                // skip the first byte and proceed with a new sequence
                next3B[0] = next3B[1];
                next3B[1] = next3B[2];
                next_byte_start = 2;
            }
            else {
                // max sequence lenght & position
                int    max_seq_lenght = 0;
                size_t max_seq_pos = -1;

                // save the current information about the file stream
                // and the history buffer for a possible future restore
                size_t fs_buf_pos = in_fs->buf_pos,
                       fs_n_available_bytes = in_fs->n_available_bytes;
                bool   fs_is_finished = in_fs->is_finished;
                size_t history_buf_pos = history_buf->next_pos;

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
                        while (!(in_fs->is_finished) && in_fs->n_available_bytes > 1) {
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

                        // if the current sequence is the longest
                        if (k > max_seq_lenght) {
                            max_seq_lenght = k + 1;
                            max_seq_pos    = last_buf_pos - buf_start_pos;
                        }

                        // restore the information of the file stream and the
                        // history buffer
                        in_fs->buf_pos = fs_buf_pos;
                        in_fs->n_available_bytes = fs_n_available_bytes;
                        in_fs->is_finished = fs_is_finished;
                        history_buf->next_pos = history_buf_pos;
                    }
                    else {
                        // this is not a valid occurrence
                        // because the first 3 bytes are not equal
                        // to the currents that we have
                    }

                    // proceed to the next position
                    chain = chain->next;
                }

                if (max_seq_pos == -1) {
                    next_byte_start = 2;
                }
                else {
                    printf("[D: %d L: %d]", (int)max_seq_pos, (int)max_seq_lenght);

                    // restore the information of the file stream and the
                    // history buffer
                    in_fs->buf_pos = fs_buf_pos + max_seq_lenght - 4;
                    in_fs->n_available_bytes = fs_n_available_bytes-max_seq_lenght + 4;
                    if (in_fs->n_available_bytes <= 0) {
                        in_fs->is_finished = true;
                    }
                    history_buf->next_pos = (history_buf_pos+max_seq_lenght) % HISTORY_BUFFER_SIZE;

                    next_byte_start = 0; // restore the start position
                }
            }
        }

        //getchar();
    }
        /*
        printf("\nHash table content: ");
        for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
            if (h_table[i] != NULL) {
                List tmp = h_table[i];
                while (tmp != NULL) {
                    printf("%d ", (int)(tmp->value));
                    tmp = tmp->next;
                }
                putchar('\n');
            }
        }
        */

    // freeing memory
    History_Buffer_destroy(history_buf);
    File_Stream_destroy(in_fs);
}
