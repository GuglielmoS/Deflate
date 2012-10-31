#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "lz.h"

void test_buffer()
{
    const char *message[] = {"Blah blah blah blah blah!",
                             "abbababac",
                             "abcdefghilmnopqrstuvz",
                             "abbabbavolpe ciao",
                             "test spazi:     passato?"};

    size_t n_msg = 5;

    printf("*** CHARACTERS STREAM TEST ***\n\n");
    for (size_t i = 0; i < n_msg; i++) {
        LZ_Stream *encoded_stream = LZ_encode_stream((uint8_t*)message[i], strlen(message[i]));
        LZ_Stream *decoded_stream = LZ_decode_stream(encoded_stream);

        printf("MESSAGE: %s\n", message[i]);

        printf("ENCODED: ");
        LZ_Stream_print(encoded_stream, stdout);

        printf("DECODED: ");
        LZ_Stream_print(decoded_stream, stdout);

        putchar('\n');
    }
}

void test_text_file()
{
    FILE *in_f = fopen("test.txt", "rb");
    FILE *out_f = fopen("test_compressed.txt", "wb");

    printf("\n*** TEXT FILE TEST ***\n\n");    

    printf("Encoding test.txt to test_compressed.txt ...\n");
    LZ_encode_file(in_f, out_f);

    in_f = fopen("test_compressed.txt", "rb");
    out_f = fopen("test_decompressed.txt", "wb");

    printf("Decoding test_compressed.txt to test_decompressed.txt ...\n");
    LZ_decode_file(in_f, out_f);

}

void test_binary_file()
{
    FILE *in_f = fopen("test.jpg", "rb");
    FILE *out_f = fopen("test_compressed.jpg", "wb");

    printf("\n*** BINARY FILE TEST ***\n\n");

    printf("Encoding test.jpg to test_compressed.jpg ...\n");
    LZ_encode_file(in_f, out_f);

    in_f = fopen("test_compressed.jpg", "rb");
    out_f = fopen("test_decompressed.jpg", "wb");

    printf("Decoding test_compressed.jpg to test_decompressed.jgp ...\n");
    LZ_decode_file(in_f, out_f);
}

int main(int argc, char *argv[])
{
    test_buffer();
    test_text_file();
    //test_binary_file();

    return EXIT_SUCCESS;
}


