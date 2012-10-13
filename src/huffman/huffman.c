#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"

HF_Tree HF_Tree_new()
{
    HF_Tree tree = (HF_Tree)malloc(sizeof(HF_Node));
    
    if (tree == NULL) {
        fprintf(stderr, "[ERROR-HF_new_tree] malloc failed!");
        exit(EXIT_FAILURE);
    }

    return tree;
}

HF_Codes_Table HF_Codes_Table_new()
{
    HF_Codes_Table table = (HF_Codes_Table)malloc(sizeof(HF_Code));

    if (table == NULL) {
        fprintf(stderr, "[ERROR-HF_new_codes_table] malloc failed!");
        exit(EXIT_FAILURE);
    }

    return table;
}

HF_Frequency* HF_get_frequencies(const char *message)
{
    HF_Frequency *freqs = (HF_Frequency*)calloc(N_SYMBOLS, sizeof(HF_Frequency)); 
    
    if (freqs == NULL) {
        fprintf(stderr, "[ERROR-HF_get_frequency] calloc failed!");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; message[i] != 0x00; i++) {
        freqs[(size_t)message[i]]++;
    }

    return freqs;
}

// for qsort
int HF_Node_compare(const void* a, const void* b)
{
    if (((const HF_Tree)a)->freq < ((const HF_Tree)b)->freq) {
        return 1;
    }
    else {
        return 0;
    }
}

HF_Tree HF_Tree_build(const HF_Frequency* freqs)
{
    HF_Tree syms[N_SYMBOLS];
    
    // creates leaf nodes by looking at frequencies
    size_t tot_node = 0;
    for (size_t i = 0; i < N_SYMBOLS; i++) {
        if (freqs[i] > 0) {
            HF_Tree tmp_tree = HF_Tree_new();    

            tmp_tree->sym   = (HF_Symbol)i;
            tmp_tree->freq  = freqs[i];
            tmp_tree->left  = NULL;
            tmp_tree->right = NULL;

            syms[tot_node++] = tmp_tree;
        }
    }
    
    // builds the tree
    while (tot_node > 1) {
        size_t cur = tot_node-1;
        
        qsort(syms, tot_node, sizeof(HF_Tree), HF_Node_compare);
        
        HF_Tree new_tree = HF_Tree_new();

        new_tree->sym   = 0x00;
        new_tree->freq  = syms[cur-1]->freq + syms[cur]->freq;
        new_tree->left  = syms[cur];
        new_tree->right = syms[cur-1]; 

        syms[cur-1] = new_tree;
        tot_node--;
    }

    return syms[0];
}

void HF_table_helper(HF_Tree t, size_t n, HF_Codes_Table table, size_t *cur_pos)
{
    static uint8_t tmp[N_SYMBOLS*2] = {0};

    if (t != NULL) {
        if (!(t->left) && !(t->right)) {
            table[*cur_pos].sym    = t->sym;
            table[*cur_pos].value  = 0x00;
            table[*cur_pos].length = n;
            (*cur_pos)++;
        }
        else {
            if (t->left) {
                tmp[n] = 0;
                HF_table_helper(t->left, n+1, table, cur_pos);
            }

            if (t->right) {
                tmp[n] = 1;
                HF_table_helper(t->right, n+1, table, cur_pos);
            }
        }
    }
}

HF_Codes_Table HF_build_huffman_table(HF_Tree t, size_t *table_size)
{
    HF_Codes_Table table = HF_Codes_Table_new();

    HF_table_helper(t, 0, table, table_size);
    
    return table;
}

int HF_Code_compare(const void* a, const void* b)
{
    size_t l1 = ((const HF_Code*)a)->length,
           l2 = ((const HF_Code*)b)->length;

    if (l1 > l2) {
        return 1;
    }
    else if (l1 == l2) {
        return ((const HF_Code*)a)->sym > ((const HF_Code*)b)->sym;
    }
    else {
        return 0;
    }
}

HF_Codes_Table HF_build_canonical_table(HF_Tree t, size_t *table_size)
{
    HF_Codes_Table table = HF_build_huffman_table(t, table_size);

    // order by length
    qsort(table, *table_size, sizeof(HF_Code), HF_Code_compare);

    // the first value is set to a number of 0 equal
    // to the length of the previous code
    // P.S.: the first value is the code which has the minor
    //       length
    table[0].value = 0x00;
    for (size_t i = 1; i < *table_size; i++) {
        uint8_t shift_len = table[i].length - table[i-1].length;
        table[i].value = (table[i-1].value + 1) << shift_len;
    }

    return table;
}
