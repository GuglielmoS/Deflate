#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "deflate.h"
#include "lz77.h"

int main(int argc, char *argv[])
{
    const char *message[] = {"Blah blah blah blah blah!",
                             "aabcbbabc"};
    size_t n_msg = 2;

    for (size_t i = 0; i < n_msg; i++) {
        printf("INPUT: %s\n", message[i]);
        printf("OUTPUT: ");
        LZ_stream_pretty_print(LZ_encode(message[i]), stdout);
        putchar('\n');
    }

    return EXIT_SUCCESS;
}

