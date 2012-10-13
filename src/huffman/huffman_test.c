#include <stdio.h>

#include "huffman.h"

/**
 * Shows the first 'bits' bit of 'c'.
 */
void show_code(uint32_t c, uint8_t bits)
{
    uint32_t mask = 1 << (bits - 1);

    for (int i = 1; i <= bits; i++) {
        putchar(c & mask ? '1' : '0');
        c <<= 1;
    }
    putchar('\n');
}

int main(int argc, char *argv[])
{
    const char *messages[] = {"AAAABBCCDE",
                              "AAAAADDDBC",
                              "ABCDE"};
    size_t n_msg = 3;

    for (size_t i = 0; i < n_msg; i++) {
        HF_Frequency  *freqs = HF_get_frequencies(messages[i]);
        HF_Tree        tree  = HF_Tree_build(freqs);
     
        size_t ctable_size = 0;
        HF_Codes_Table ctable = HF_build_canonical_table(tree, &ctable_size);

        printf("Message: %s\n", messages[i]);
        printf("Codes:   \n");
        for (size_t i = 0; i < ctable_size; i++) {
            printf("%c -> ", ctable[i].sym);
            show_code(ctable[i].value, ctable[i].length);
        }
    }
     
    return 0;
}
