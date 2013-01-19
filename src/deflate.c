#include "deflate.h"

/**
 * Decodes the current queue and writes it out on the file related to f_out.
 * It assumes that f_out is a valid file descriptor referred to a file opened in
 * binary writing mode.
 */
void LZ_decode_process_queue(LZ_Queue *queue, FILE *f_out)
{
    size_t buf_size = 0;
    static uint8_t buf[INPUT_BLOCK_SIZE];

    // processes the queue
    while (!LZQ_IS_EMPTY(queue)) {
        LZ_Element *next_el = LZQ_DEQUEUE(queue);

        if (LZE_IS_LITERAL(next_el)) {
            buf[buf_size++] = LZE_GET_LITERAL(next_el);
        }
        else {
            size_t init_pos  = buf_size - LZE_GET_DISTANCE(next_el);
            for (size_t i = 0; i < LZE_GET_LENGTH(next_el); i++) {
                buf[buf_size++] = buf[init_pos + i];
            }
        }

        // free the processed element
        free(next_el);
    }

    // writes the decoded buffer on the file
    WRITE_BYTES(f_out, buf, buf_size);
}

/**
 * Process the LZ_Queue 'queue' and writes the output on the stream bs_out.
 * bs_out must be a valid Bit_Stream open in binary write mode.
 *
 * By defult it performs a compression with static huffman.
 * */
void Deflate_process_queue(LZ_Queue *queue, Bit_Stream *bs_out, bool last_block)
{
    // if is the last block, puts a '1' bit, '0' otherwise
    Bit_Stream_add_bit(bs_out, (last_block == true));

    // puts the other two bits which state the type of block
    Bit_Stream_add_bit(bs_out, 0); // STATIC
    Bit_Stream_add_bit(bs_out, 1); // HUFFMAN

    //Bit_Vec *tmp_code = NULL;
    //LZ_Element *next_el = NULL;

    // processes the queue of LZ elements
    while (!LZQ_IS_EMPTY(queue)) {
        LZ_Element *next_el = LZQ_DEQUEUE(queue);
        Bit_Vec *tmp_code = Bit_Vec_create();

        // outputs the literal code
        if (LZE_IS_LITERAL(next_el)) {
            HUFFMAN_GET_LITERAL_CODE(LZE_GET_LITERAL(next_el), tmp_code);
            BIT_STREAM_ADD_BIT_VEC(bs_out, tmp_code);
        }
        // outputs the distance/length pair code
        else {
            // gets the length code
            Huffman_get_length_code(LZE_GET_LENGTH(next_el), tmp_code);
            BIT_STREAM_ADD_BIT_VEC(bs_out, tmp_code);

            // resets the temporary bits vector
            Bit_Vec_destroy(tmp_code);
            free(tmp_code);
            tmp_code = Bit_Vec_create();

            // gets the distance code
            Huffman_get_distance_code(LZE_GET_DISTANCE(next_el), tmp_code);
            BIT_STREAM_ADD_BIT_VEC(bs_out, tmp_code);
        }

        // frees the memory used for the LZ_Element and the temporary Bit_Vec
        Bit_Vec_destroy(tmp_code);
        free(tmp_code);
        free(next_el);
    }

    // adds the end of block (edoc 256 => 000 0000)
    for (int i = 0; i < 7; i++) {
        Bit_Stream_add_bit(bs_out, 0);
    }

    // if it's the last block, it forces the write process on the file
    if (last_block) {
        Bit_Stream_force_write(bs_out);
        Bit_Stream_write_padding_bits(bs_out);
    }
}

/**
 * Decompresses the file identified by params->in_file_name to a new
 * file named params->out_file_name.
 */
void Deflate_decode(Deflate_Params *params)
{
    // initializes the input bits stream
    Bit_Stream in_s;
    Bit_Stream_init(&in_s, params->in_file_name, "rb", 8192);

    // opens the output file
    FILE *f_out = fopen(params->out_file_name, "wb");
    if (f_out == NULL) {
        die_error("[ERROR-Deflate_decode] can't open output file!\n");
    }

    // initializes the queue for the LZ_Elements
    LZ_Queue lz_queue;
    LZ_Queue_init(&lz_queue);

    // used to determine when we need to stop the decoding process
    bool last_block_processed = false;

    // continues until the last block in the file is processed
    while (!last_block_processed) {

        // reads the first bit of the block which states
        // if we have to process the last block in the file or not
        last_block_processed = Bit_Stream_get_bit(&in_s) == 1;

        // reads the block type
        uint8_t block_type = 0x00;
        if (Bit_Stream_get_bit(&in_s))
            block_type |= 0x02;
        if (Bit_Stream_get_bit(&in_s))
            block_type |= 0x01;

        // processes the block util it finds the block separator sequence
        // BLOCK SEPARATOR = 0000000

        // static huffman decoding
        if (block_type == STATIC_HUFFMAN_TYPE) {

            bool block_finished = false;
            while (!block_finished) {

                // gets the first seven bits to determine what kind of
                // symbol we have to process, and thus how many extra bits
                // we need to read
                uint16_t cur_code = 0x0000;
                for (int i = 0; i < 7; i++) {
                    cur_code <<= 1;
                    if (Bit_Stream_get_bit(&in_s)) {
                        cur_code |= 0x0001;
                    }
                }

                uint8_t n_extra_bits = 0x00;
                if      (cur_code <= 23) n_extra_bits = 0;
                else if (cur_code <= 95) n_extra_bits = 1;
                else if (cur_code <= 99) n_extra_bits = 1;
                else                     n_extra_bits = 2;

                // reads the 'extra' bits of the current symbol based on
                // the value read just over here
                for (int i = 0; i < n_extra_bits; i++) {
                    cur_code <<= 1;
                    if (Bit_Stream_get_bit(&in_s)) {
                        cur_code |= 0x0001;
                    }
                }

                // normalizes the result symbol value's by calcuting the
                // relative 'edoc'
                int edoc = 0;
                if      (cur_code <= 23)  edoc = cur_code + 256;
                else if (cur_code <= 191) edoc = cur_code - 48;
                else if (cur_code <= 199) edoc = cur_code + 88;  // - 192 + 280;
                else                      edoc = cur_code - 256; // - 400 + 144;

                // now that we know the 'edoc', we can certainly determine which
                // operations are needed for completing the decompression of
                // the current symbol
                if (edoc == 256) {
                    // BLOCK SEPARATOR => 000 0000
                    block_finished = true;
                }
                else if (edoc <= 255) {
                    LZQ_ENQUEUE_LITERAL(&lz_queue, edoc);
                }
                else {
                    // length extra bits
                    uint8_t  l_extra_bits = 0x00;
                    uint16_t l_pos = edoc - 257;
                    for (int i = 0; i < lext[l_pos]; i++) {
                        l_extra_bits <<= 1;
                        if (Bit_Stream_get_bit(&in_s)) {
                            l_extra_bits |= 0x01;
                        }
                    }

                    // distance position in the code table
                    uint8_t d_pos = 0x00;
                    for (int i = 0; i < 5; i++) {
                        d_pos <<= 1;
                        if (Bit_Stream_get_bit(&in_s)) {
                            d_pos |= 0x01;
                        }
                    }

                    // distance extra bits
                    uint16_t d_extra_bits = 0x0000;
                    for (int i = 0; i < dext[d_pos]; i++) {
                        d_extra_bits <<= 1;
                        if (Bit_Stream_get_bit(&in_s)) {
                            d_extra_bits |= 0x0001;
                        }
                    }

                    // enqueues the resultant LZ_Pair
                    LZQ_ENQUEUE_PAIR(&lz_queue, dists[d_pos] + d_extra_bits,
                                                lens[l_pos]  + l_extra_bits);
                }
            }
        }

        // decodes the current queue to the output file
        LZ_decode_process_queue(&lz_queue, f_out);
    }

    // closes the files stream
    fclose(f_out);
    Bit_Stream_close(&in_s);
    Bit_Stream_destroy(&in_s);
}

/**
 * Compresses the content of the file identified by 'params->in_file_name' by applying
 * the deflate algorithm defined in the RFC1951.
 * The output will be written to the file 'params->out_file_name'.
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
    Bit_Stream_init(&out_s, params->out_file_name, "wb", INPUT_BLOCK_SIZE);

    // current input data block
    uint8_t *cur_block = (uint8_t*)malloc(INPUT_BLOCK_SIZE*sizeof(uint8_t));
    if (cur_block == NULL) {
        die_error("[ERROR-Deflate_encode] malloc failed!\n");
    }

    // lookup table used for searching in the buffer
    Hash_Table lookup_table = (Limited_List*)malloc(HASH_TABLE_SIZE*sizeof(Limited_List));
    if (lookup_table == NULL) {
        die_error("[ERROR-Deflate_encode] malloc failed!\n");
    }

    // queue used for processing the LZ77 output
    LZ_Queue lz_queue;

    // next 3 bytes in the input file
    uint8_t next3B[3];

    // states if we are processing the last block
    bool last_block = false;

    // initializes the lookup table and the lz_queue
    LZ_Queue_init(&lz_queue);
    Hash_Table_init(lookup_table, HASH_TABLE_MAX_LIST_LEN);

    // processes the blocks
    size_t block_size = 0;
    while ((block_size = READ_BLOCK(cur_block,in_f)) > 0) {
        if (block_size < INPUT_BLOCK_SIZE)
            last_block = true;

        // look-ahead buffer start position
        size_t lab_start = 0;

        // starts the search in the buffer
        while (lab_start < block_size) {

            // retrieves the next 3 bytes in the buffer
            size_t i = 0;
            while (lab_start + i < block_size && i < 3) {
                next3B[i] = cur_block[lab_start+i];
                i++;
            }

            if (i < 3) {
                // we are at the end of the block, thus we put the last literals
                // in the queue
                for (size_t j = 0; j < i; j++) {
                    LZQ_ENQUEUE_LITERAL(&lz_queue, next3B[j]);
                }

                break;
            }
            else {
                // retrieves the positions of the current sequence (next3B)
                Limited_List *chain = HTABLE_GET(lookup_table, next3B);

                if (chain->values == NULL) {
                    // the chain is empty, so we put the current three bytes
                    // and their position in it
                    HTABLE_PUT(lookup_table, next3B, lab_start);

                    // outputs the current byte
                    LZQ_ENQUEUE_LITERAL(&lz_queue, next3B[0]);

                    // advances of one position in the buffer
                    lab_start++;
                }
                else {
                    // temporary variables used to store the longest match
                    // sequence position and length.
                    size_t longest_match_length = 0,
                           longest_match_pos = -1;

                    // goes through the chain to find the longest match
                    for (size_t i = 0; i < chain->cur_size; i++) {
                        // position of the match in the search buffer
                        size_t match_pos = LIMITED_LIST_GET(chain, i);

                        // length of the current occurrence
                        size_t k = 0;

                        // finds the length of the current occurrence
                        while (lab_start + k < block_size     &&
                                           k < LZ_MAX_SEQ_LEN &&
                               cur_block[match_pos + k] == cur_block[lab_start + k]) {
                            k++;
                        }

                        // if there is a match, checks if it's the longest
                        if (k > longest_match_length) {
                            longest_match_pos    = match_pos;
                            longest_match_length = k;
                        }
                    }

                    // if the sequence isn't long enough
                    if (longest_match_length < LZ_MIN_SEQ_LEN) {
                        // outputs the current byte
                        LZQ_ENQUEUE_LITERAL(&lz_queue, next3B[0]);

                        if (params->fast == false) {
                            // updates the lookup table with the current three bytes
                            HTABLE_PUT(lookup_table, next3B, lab_start);
                        }

                        // advances of one position in the buffer
                        lab_start++;
                    }
                    else {
                        // outputs the current LENGTH/DISTANCE pair
                        LZQ_ENQUEUE_PAIR(&lz_queue, lab_start - longest_match_pos,
                                                    longest_match_length);

                        if (params->fast == false) {
                            // updates the lookup table with the bytes between the
                            // look-ahead buffer init position and the new look-ahead
                            // buffer position
                            size_t final_pos = lab_start + longest_match_length;
                            while (lab_start < final_pos - 2) {
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
                    }
                }
            }
        }

        // processes the current queue
        Deflate_process_queue(&lz_queue, &out_s, last_block);

        // resets the lookup table
        Hash_Table_reset(lookup_table);
    }

    // frees the memory allocated previously for the lookup table and the data input block
    free(lookup_table);
    free(cur_block);

    // closes and destroys the bits stream; thus close the input file
    Bit_Stream_destroy(&out_s);
    Bit_Stream_close(&out_s);
    fclose(in_f);
}

