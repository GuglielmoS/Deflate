#include <stdio.h>

#include "huffman.h"
#include "../util.h"

int main(int argc, char *argv[])
{
    const char *messages[] = {"AAAABBCCDE",
                              "AAAAADDDBC",
                              "ABCDE",
                              "AB CB A B  "};
    size_t n_msg = 4;

    for (size_t i = 0; i < n_msg; i++) {
        HF_Frequency  *freqs = HF_get_frequencies(messages[i]);
        HF_Tree        tree  = HF_Tree_build(freqs);

        size_t ctable_size = 0;
        HF_Codes_Table ctable = HF_build_canonical_table(tree, &ctable_size);

        printf("Message: %s\n", messages[i]);
        printf("Codes:   \n");
        for (size_t i = 0; i < ctable_size; i++) {
            printf("%c -> ", ctable[i].sym);
            show_bits(ctable[i].value, ctable[i].length);
        }
    }

    return 0;
}
