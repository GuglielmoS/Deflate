#include "deflate.h"


/**
 * Decode the current queue and writes it out on the file related to f_out.
 * It assumes that f_out is a valid file descriptor referred to a file opened in
 * binary writing mode.
 */
void LZ_decode_process_queue(LZ_Queue *queue, FILE *f_out)
{
    size_t buf_size = 0;
    uint8_t buf[INPUT_BLOCK_SIZE];

    LZ_Element *next_el = NULL;
    while (!LZQ_IS_EMPTY(queue)) {
        next_el = LZQ_DEQUEUE(queue);

        if (LZE_IS_LITERAL(next_el)) {
            buf[buf_size++] = LZE_GET_LITERAL(next_el);

#ifdef DEBUG
            printf("%c", LZE_GET_LITERAL(next_el));
#endif
        }
        else {
 #ifdef DEBUG
            printf("[L: %d, D: %d]", LZE_GET_LENGTH(next_el), LZE_GET_DISTANCE(next_el));
#endif

            if (LZE_GET_DISTANCE(next_el) > buf_size) {
                printf("%d\n", LZE_GET_DISTANCE(next_el));
            }

            size_t init_pos = buf_size - LZE_GET_DISTANCE(next_el);
            for (size_t i = 0; i < LZE_GET_LENGTH(next_el); i++) {
                buf[buf_size++] = buf[init_pos + i];
            }
        }

        free(next_el);
    }

    // writes the decoded buffer on the file
    WRITE_BYTES(f_out, buf, buf_size);
}

/**
 * Process the LZ_Queue 'queue' and writes the output on the stream bs_out.
 * bs_out must be a valid Bit_Stream open in binary write mode.
 * By defult it performs a compression with static huffman, however if
 * the statistics aren't good for this kind of method, it builds a new huffman
 * codes table and uses it to compress the queue.
 */
void Deflate_process_queue(LZ_Queue *queue, Statistics *stats, Bit_Stream *bs_out, bool last_block)
{
    // if is the last block, puts a '1' bit, '0' otherwise
    Bit_Stream_add_bit(bs_out, (last_block == true));

    // puts the other two bits which state the type of block
    Bit_Stream_add_bit(bs_out, 0); // STATIC
    Bit_Stream_add_bit(bs_out, 1); // HUFFMAN

    Bit_Vec *tmp_code = NULL;
    LZ_Element *next_el = NULL;

#ifdef DEBUG
    printf("%d | %d%d |\n\n", Bit_Vec_get_bit(bs_out->bits_buf,0),
                              Bit_Vec_get_bit(bs_out->bits_buf,1),
                              Bit_Vec_get_bit(bs_out->bits_buf,2));
#endif

    // processes the LZ queue
    while (!LZQ_IS_EMPTY(queue)) {
        next_el = LZQ_DEQUEUE(queue);
        tmp_code = Bit_Vec_create();

        if (LZE_IS_LITERAL(next_el)) {
            Huffman_get_literal_code(LZE_GET_LITERAL(next_el), tmp_code);
            Bit_Stream_add_n_bit(bs_out, tmp_code);

#ifdef DEBUG
            printf("%c -> ", LZE_GET_LITERAL(next_el));
            for (int i = 0; i < tmp_code->cur_size; i++) {
                printf("%d", Bit_Vec_get_bit(tmp_code, i));
            }
            putchar('\n');
#endif

            Bit_Vec_destroy(tmp_code);
            free(tmp_code);
        }
        else {
            Huffman_get_length_code(LZE_GET_LENGTH(next_el), tmp_code);
            Bit_Stream_add_n_bit(bs_out, tmp_code);

            Bit_Vec_destroy(tmp_code);
            free(tmp_code);

            tmp_code = Bit_Vec_create();
            Huffman_get_distance_code(LZE_GET_DISTANCE(next_el), tmp_code);
            Bit_Stream_add_n_bit(bs_out, tmp_code);

            Bit_Vec_destroy(tmp_code);
            free(tmp_code);
        }

        free(next_el);
    }

    // adds the end of block (edoc 256 => 000 0000)
    Bit_Vec *sep_code = Bit_Vec_create();
    Huffman_get_end_block_separator(sep_code);
    Bit_Stream_add_n_bit(bs_out, sep_code);

    Bit_Vec_destroy(sep_code);
    free(sep_code);

    // if it's the last block, it forces the write process on the file
    if (last_block) {
        Bit_Stream_force_write(bs_out);
        Bit_Stream_write_padding_bits(bs_out);
    }
}

/**
 * Compress the content of the file identified by 'in_file_name' by applying
 * the deflate algorithm defined in the RFC1951.
 * The output will be written to the file 'out_file_name'.
 */
void Deflate_encode(Deflate_Params *params)
{
    FILE *in_f = fopen(params->in_file_name, "rb");

    // open the file to compress
    if (in_f == NULL) {
        die_error("[ERROR-Deflate_encode] can't open the input file!\n");
    }

    // output bits stream
    Bit_Stream out_s;
    Bit_Stream_init(&out_s, params->out_file_name, "wb", OUTPUT_BLOCK_SIZE);

    // current input data block
    uint8_t *cur_block = (uint8_t*)malloc(INPUT_BLOCK_SIZE*sizeof(uint8_t));
    if (cur_block == NULL) {
        die_error("[ERROR-Deflate_encode] failed malloc!\n");
    }

    size_t block_size = 0, // current block size
           lab_start = 0;  // look-ahead buffer start position

    Hash_Table lookup_table; // lookup table used for searching in the buffer

    lookup_table = (Limited_List*)malloc(HASH_TABLE_SIZE*sizeof(Limited_List));
    if (lookup_table == NULL) {
        die_error("[ERROR-Deflate_encode] failed malloc!\n");
    }

    LZ_Queue lz_queue; // queue used for processing the LZ77 output

    uint8_t next3B[3]; // next 3 bytes in the input file

    bool last_block = false; // states if we are at the last block

    // initialize the lookup table and the lz_queue
    LZ_Queue_init(&lz_queue);
    Hash_Table_init(lookup_table);

    // processes the blocks
    while ((block_size = READ_BLOCK(cur_block,in_f)) > 0) {
        // for data statistics (frequencies, literals/pair count ...)
        //Statistics stats = {0, 0, {0}};

        if (block_size < INPUT_BLOCK_SIZE)
            last_block = true;

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

                    // updates the statistics
                    //STATS_INC_FREQ(stats, next3B[j]);
                    //STATS_INC_LIT(stats);
                }

                break;
            }
            else {
                List chain = HTABLE_GET(lookup_table,next3B);

                if (chain == NULL) {
                    // the chain is empty, so we put the current three bytes
                    // and their position in it
                    HTABLE_PUT(lookup_table, next3B, lab_start);

                    // outputs the current byte
                    LZQ_ENQUEUE_LITERAL(lz_queue,next3B[0]);

                    // updates the statistics
                    //STATS_INC_FREQ(stats, next3B[0]);
                    //STATS_INC_LIT(stats);

                    // advances of one position
                    lab_start++;
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

                        // proceed with the next match
                        chain = chain->next;
                    }

                    if (longest_match_length < LZ_MIN_SEQ_LEN) {
                        // output the current byte and advances of one position
                        LZQ_ENQUEUE_LITERAL(lz_queue,next3B[0]);

                        if (params->fast == false) {
                            // updates the lookup table with the current three bytes
                            HTABLE_PUT(lookup_table, next3B, lab_start);
                        }

                        // updates the statistics
                        //STATS_INC_FREQ(stats, next3B[0]);
                        //STATS_INC_LIT(stats);

                        // advances of one position
                        lab_start++;
                    }
                    else {
                        LZQ_ENQUEUE_PAIR(lz_queue, lab_start - longest_match_pos,
                                                   longest_match_length);

                        if (params->fast == false) {
                            // updates the lookup table with the bytes between the
                            // look-ahead buffer init position and the new look-ahead
                            // buffer position
                            size_t final_pos = lab_start + longest_match_length;
                            while (lab_start < final_pos-2) {
                                for (size_t i = 0; i < 3; i++) {
                                    next3B[i] = cur_block[lab_start+i];
                                }

                                HTABLE_PUT(lookup_table, next3B, lab_start);
                                lab_start++;
                            }
                            lab_start += 2;
                        }
                        else {
                            lab_start += longest_match_length;
                        }

                        // updates the statistics
                        //STATS_INC_PAIR(stats);
                    }
                }
            }
        }

        // processes the current queue
        Deflate_process_queue(&lz_queue, NULL, &out_s, last_block);
        Hash_Table_reset(lookup_table);
    }

    free(lookup_table);
    free(cur_block);
    fclose(in_f);

    Bit_Stream_destroy(&out_s);
    Bit_Stream_close(&out_s);
}

/**
 * Decompresses the file identified by params->in_file_name to a new
 * file named params->out_file_name.
 */
void Deflate_decode(Deflate_Params *params)
{
    Bit_Stream in_s;
    Bit_Stream_init(&in_s, params->in_file_name, "rb", INPUT_BLOCK_SIZE);

    FILE *f_out = fopen(params->out_file_name, "wb");
    if (f_out == NULL) {
        die_error("[ERROR-Deflate_decode] can't open output file!\n");
    }

    LZ_Queue lz_queue;
    LZ_Queue_init(&lz_queue);

    bool last_block = false;

    while (!Bit_Stream_finished(&in_s)) {

        last_block = Bit_Stream_get_bit(&in_s);
        Bit_Stream_get_bit(&in_s); // BLOCK
        Bit_Stream_get_bit(&in_s); // TYPE

        bool block_finished = false;
        while (!block_finished) {
            uint8_t init_code = 0x00;
            for (int i = 0; i < 7; i++) {
                if (Bit_Stream_get_bit(&in_s)) {
                    init_code = BYTE_BIT_SET(init_code, 6-i);
                }
            }

            uint16_t final_code = init_code;
            int n_extra_bits = Huffman_get_decode_extra_bits(init_code);
            for (int i = 0; i < n_extra_bits; i++) {
                final_code <<= 1;
                if (Bit_Stream_get_bit(&in_s)) {
                    final_code = WORD_BIT_SET(final_code, 0);
                }
            }

            uint16_t edoc = final_code - Huffman_get_decode_offset(init_code);

            if (init_code <= 23) edoc += 256;
            else if (init_code <= 95) edoc += 0;
            else if (init_code <= 99) edoc += 280;
            else edoc += 144;

            if (edoc == 256) {
#ifdef DEBUG
                printf("BLOCK FINISHED!\n");
#endif
                block_finished = true;
            }
            else if (edoc <= 255) {
#ifdef DEBUG
                printf("-> '%c'\n\n", edoc);
#endif
                LZQ_ENQUEUE_LITERAL(lz_queue,edoc);
            }
            else {
#ifdef DEBUG
                printf("LENGTH & PAIR TO PROCESS\n");
#endif

                uint8_t extra_bits = 0x00;
                uint8_t len_pos = edoc - 257;
                for (int i = 0; i < lext[len_pos]; i++) {
                    extra_bits <<= 1;
                    if (Bit_Stream_get_bit(&in_s)) {
                        extra_bits = BYTE_BIT_SET(extra_bits, 0);
                    }
                }
                uint16_t length = lens[len_pos]+extra_bits;
#ifdef DEBUG
                printf("-> L: %d, ", length);
#endif

                uint8_t d_pos = 0x00;
                for (int i = 0; i < 5; i++) {
                    if (Bit_Stream_get_bit(&in_s)) {
                        d_pos = BYTE_BIT_SET(d_pos, 4-i);
                    }
                }

                extra_bits = 0x00;
                for (int i = 0; i < dext[d_pos]; i++) {
                    extra_bits <<= 1;
                    if (Bit_Stream_get_bit(&in_s)) {
                        extra_bits = BYTE_BIT_SET(extra_bits, 0);
                    }
                }

                uint16_t distance = dists[d_pos] + extra_bits;
#ifdef DEBUG
                printf("D: %d\n", distance);
#endif
                LZQ_ENQUEUE_PAIR(lz_queue,distance,length);
            }
        }

        LZ_decode_process_queue(&lz_queue, f_out);
    }

    fclose(f_out);

    Bit_Stream_close(&in_s);
    Bit_Stream_destroy(&in_s);
}
