#ifndef __HUFFMAN__
#define __HUFFMAN__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// max number of symbols
// it's 255 because we encode one byte at time
// so 2^8 => 255 possible symbols
#define N_SYMBOLS 255

typedef uint8_t  HF_Symbol;    // type used for symbols
typedef uint32_t HF_Frequency; // type used for frequencies

/**
 * Represents a node of the Huffman tree.
 */
typedef struct __hf_node {
    HF_Symbol         sym;          // symbol
    HF_Frequency      freq;         // symbol's frequency
    struct __hf_node *left, *right; // tree childs
} HF_Node;

// an useful definition
typedef HF_Node* HF_Tree;

/**
 * Represents a huffman code associated to a symbol. 
 */
typedef struct {
    HF_Symbol sym;     // symbol associated to the code
    uint8_t  value;    // code packed in a byte
    uint8_t  length;   // length of the code in bits (max 255 symbols)
} HF_Code;

// huffman codes table
typedef HF_Code* HF_Codes_Table;

HF_Tree HF_Tree_build(const HF_Frequency* freqs);
HF_Frequency*  HF_get_frequencies(const char *message);
HF_Codes_Table HF_build_canonical_table(HF_Tree t, size_t *table_size);

#endif /* __HUFFMAN__ */
