#ifndef __DEFLATE__
#define __DEFLATE__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "list.h"
#include "util.h"
#include "lz_queue.h"
#include "lz_element.h"
#include "hash_table.h"

#define INPUT_BLOCK_SIZE 32768

#define LZ_MIN_SEQ_LEN 3
#define LZ_MAX_SEQ_LEN 258

/*** USEFUL MACROS ***/

// writes a byte (uint8_t) on the file 'fp'
#define WRITE_BYTE(fp,b) fwrite((uint8_t*)&b, sizeof(uint8_t), 1, fp)

// reads the next block from the file identified by 'fd'
#define READ_BLOCK(block,fd) fread((uint8_t*)(block), sizeof(uint8_t), INPUT_BLOCK_SIZE, (fd))

/*** FUNCTIONS ***/
void LZ_encode(const char *in_file_name, const char *out_file_name);
void Deflate_encode(const char *in_file_name, const char *out_file_name);
void Deflate_encode(const char *in_file_name, const char *out_file_name);

#endif /* __DEFLATE__ */
