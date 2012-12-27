#include "deflate.h"

/**
 * Decode the current queue and writes it out on the file related to f_out.
 * It assumes that f_out is a valid file pointer referred to a file open in
 * binary write mode.
 */
void LZ_decode_process_queue(LZ_Queue *queue, FILE *f_out)
{
    if (f_out != NULL ) {
        uint8_t tmp_buf[INPUT_BLOCK_SIZE];
        size_t tmp_buf_pos = 0;

        while (!LZQ_IS_EMPTY(queue)) {
            LZ_Element *next_el = LZQ_DEQUEUE(queue);

            if (LZE_IS_LITERAL(next_el)) {
                uint8_t byte = LZE_GET_LITERAL(next_el);

                tmp_buf[tmp_buf_pos++] = byte;
                WRITE_BYTE(f_out, byte);
            }
            else {
                size_t init_pos = tmp_buf_pos - LZE_GET_DISTANCE(next_el);
                for (size_t i = 0; i < LZE_GET_LENGTH(next_el); i++) {
                    uint8_t byte = tmp_buf[init_pos + i];

                    tmp_buf[tmp_buf_pos++] = byte;
                    WRITE_BYTE(f_out, byte);
                }
            }
        }
    }
}

/**
 * Compress the content of the file identified by 'in_file_name' by applying
 * the deflate algorithm defined in the RFC1950.
 * The output will be written to the file 'out_file_name'.
 */
void Deflate_encode(const char *in_file_name, const char *out_file_name)
{
    FILE *in_f = fopen(in_file_name, "rb");

    // open the file to compress
    if (in_f == NULL) {
        die_error("[ERROR-Deflate_encode] can't open the input file!\n");
    }

    FILE *out_f = fopen(out_file_name, "wb");
    if (out_f == NULL) {
        fclose(in_f);
        die_error("[ERROR-Deflate_encode] can't open/create the output file!\n");
    }

    // current input data block
    uint8_t cur_block[INPUT_BLOCK_SIZE];

    size_t block_size = 0, // current block size
           lab_start = 0;  // look-ahead buffer start position

    Hash_Table lookup_table; // lookup table used for searching in the buffer

    LZ_Queue lz_queue; // queue used for processing the LZ77 output

    uint8_t next3B[3]; // next 3 bytes in the input file

    // for statistics
    size_t lit_count = 0, pair_count = 0;

    while ((block_size = READ_BLOCK(cur_block,in_f)) > 0) {
        LZ_Queue_init(&lz_queue);
        Hash_Table_init(lookup_table);

        lab_start = 0;
        while (lab_start < block_size) {
            size_t i = 0;
            while (lab_start+i < block_size && i < 3) {
                next3B[i] = cur_block[lab_start+i];
                i++;
            }

            if (i < 3) {
                // we are at the end of the block, so we put the last literals
                // in the queue
                for (size_t j = 0; j < i; j++) {
                    LZQ_ENQUEUE_LITERAL(lz_queue,next3B[j]);
                    lit_count++;
                }

                break;
            }
            else {
                List chain = Hash_Table_get(lookup_table, next3B);

                if (chain == NULL) {
                    // the chain is empty, so we put the current three bytes
                    // and their position in it
                    Hash_Table_put(lookup_table, next3B, lab_start);

                    // output the current byte and advances of one position
                    LZQ_ENQUEUE_LITERAL(lz_queue,next3B[0]);
                    lab_start++;

                    lit_count++;
                }
                else {
                    size_t longest_match_length = 0,
                           longest_match_pos = -1;

                    // go through the chain to find the longest match
                    while (chain != NULL) {
                        // position of the match in the search buffer
                        size_t match_pos = chain->value;

                        // length of the current occurrence
                        size_t k = 0;

                        // finds the length of the current occurrence
                        while (lab_start + k < block_size     &&
                                           k < LZ_MAX_SEQ_LEN &&
                               cur_block[match_pos+k] == cur_block[lab_start+k]) {
                            k++;
                        }

                        // if there is a match, checks if it's the longest
                        if (k > longest_match_length) {
                            longest_match_length = k;
                            longest_match_pos = match_pos;
                        }

                        // process with the next match
                        chain = chain->next;
                    }

                    if (longest_match_length < LZ_MIN_SEQ_LEN) {
                        // TODO: PERFORMANCE PROBLEM HERE!
                        //Hash_Table_put(lookup_table, next3B, lab_start);

                        // output the current byte and advances of one position
                        LZQ_ENQUEUE_LITERAL(lz_queue,next3B[0]);
                        lab_start++;

                        lit_count++;
                    }
                    else {
                        LZQ_ENQUEUE_PAIR(lz_queue, lab_start - longest_match_pos,
                                                   longest_match_length);
                        pair_count++;

                        lab_start += longest_match_length;

                        /*
                            PERFORMANCE PROBLEM HERE!
                        // updates the lookup table with the bytes between the
                        // look-ahead buffer init position and new look-ahead
                        // buffer position
                        size_t final_pos = lab_start + longest_match_length;
                        while (lab_start < final_pos-2) {
                            for (size_t i = 0; i < 3; i++) {
                                next3B[i] = cur_block[lab_start+i];
                            }
                            Hash_Table_put(lookup_table, next3B, lab_start);
                            lab_start++;
                        }
                        lab_start += 2;
                        */
                    }
                }
            }
        }

        LZ_decode_process_queue(&lz_queue, out_f);
        LZ_Queue_destroy(&lz_queue);

        Hash_Table_destroy(lookup_table);
    }

    printf("\nSTATS\n");
    printf("\tLIT COUNT:  %ld\n", lit_count);
    printf("\tPAIR COUNT: %ld\n", pair_count);
    printf("\tFINAL SIZE: %ld\n", lit_count+3*pair_count);

    // freeing memory
    fclose(in_f);
    fclose(out_f);
}
