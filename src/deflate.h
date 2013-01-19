#ifndef __DEFLATE__
#define __DEFLATE__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "util.h"
#include "limited_list.h"
#include "bit_vec.h"
#include "bit_stream.h"
#include "lz_queue.h"
#include "lz_element.h"
#include "hash_table.h"
#include "huffman.h"

// size of each block processed
#define INPUT_BLOCK_SIZE 32768

// maxmimum size of a temporary block
// when the Bit_Stream is used
#define OUTPUT_BLOCK_SIZE 32768

// min and max sequence length for the LZ77 algorithm
#define LZ_MIN_SEQ_LEN 3
#define LZ_MAX_SEQ_LEN 258

// maximum number of position stored in the hash table during the LZ77 procedure
#define HASH_TABLE_MAX_LIST_LEN 20

// value of STATIC HUFFMAN BLOCK'S 2-BIT HEADER
#define STATIC_HUFFMAN_TYPE 1

/*** AUXILIARIES DATA STRUCTURES ***/

// for handling the paramaters of compression/decompression
typedef struct {
    const char *in_file_name;
    const char *out_file_name;
    bool fast;
} Deflate_Params;

/*** USEFUL MACROS ***/

// writes a byte (uint8_t) on the file 'fp'
#define WRITE_BYTE(fp,b) fwrite((uint8_t*)&b, sizeof(uint8_t), 1, fp)

// writes 's' bytes from 'b' to 'fp'
#define WRITE_BYTES(fp,b,s) fwrite((uint8_t*)b,sizeof(uint8_t),s,fp)

// reads the next block from the file identified by 'fd'
#define READ_BLOCK(block,fd) fread((uint8_t*)(block), sizeof(uint8_t), INPUT_BLOCK_SIZE, (fd))

/*** FUNCTIONS ***/
void Deflate_encode(Deflate_Params *params);
void Deflate_decode(Deflate_Params *params);

#endif /* __DEFLATE__ */
