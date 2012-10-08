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
        LZ_Stream *encoded_stream = LZ_encode(message[i]);
        LZ_Stream *decoded_stream = LZ_decode(encoded_stream);

        printf("MESSAGE: %s\n", message[i]);

        printf("ENCODED: ");
        LZ_stream_print(encoded_stream, stdout);

        printf("DECODED: ");
        LZ_stream_print(decoded_stream, stdout);

        putchar('\n');
    }

    return EXIT_SUCCESS;
}

