#ifndef __DEFLATE__
#define __DEFLATE__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "list.h"
#include "util.h"

#include "bit_vec.h"
#include "bit_stream.h"

#include "lz_queue.h"
#include "lz_element.h"

#include "hash_table.h"

// size of each block processed
#define INPUT_BLOCK_SIZE 32768

// min and max sequence length for the LZ77 algorithm
#define LZ_MIN_SEQ_LEN 3
#define LZ_MAX_SEQ_LEN 258

/*** AUXILIARIES DATA STRUCTURES ***/

// represents the structure used for storing data informations
// along the process of encoding
typedef struct {
    uint32_t lit_count;  // number of literals
    uint32_t pair_count; // number of pair
    uint32_t freqs[255]; // literals frequencies
} Statistics;

// block types
typedef enum {RAW_DATA,
              STATIC_HUFFMAN,
              DYNAMIC_HUFFMAN,
              UNKNOWN}
        Block_Type;

/*** USEFUL MACROS ***/

// writes a byte (uint8_t) on the file 'fp'
#define WRITE_BYTE(fp,b) fwrite((uint8_t*)&b, sizeof(uint8_t), 1, fp)

// writes 's' bytes from 'b' to 'fp' 
#define WRITE_BYTES(fp,b,s) fwrite((uint8_t*)&b,sizeof(uint8_t),s,fp)

// reads the next block from the file identified by 'fd'
#define READ_BLOCK(block,fd) fread((uint8_t*)(block), sizeof(uint8_t), INPUT_BLOCK_SIZE, (fd))

// increments the literal count of a statistics data structure
#define STATS_INC_LIT(s) (s).lit_count++

// increments the pair count of a statistics data structure
#define STATS_INC_PAIR(s) (s).pair_count++

// increments the symbol frequency of a statistics data structure
#define STATS_INC_FREQ(s,sym) (s).freqs[(sym)]++

/*** FUNCTIONS ***/
void LZ_encode(const char *in_file_name, const char *out_file_name);
void Deflate_encode(const char *in_file_name, const char *out_file_name);
void Deflate_encode(const char *in_file_name, const char *out_file_name);

#endif /* __DEFLATE__ */
