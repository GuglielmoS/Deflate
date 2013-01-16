#include <stdio.h>
#include "bit_vec.h"
#include "huffman.h"

void print_bit_vec(Bit_Vec *bv)
{
    for (int i = 0; i < bv->cur_size; i++) {
        printf("%d", Bit_Vec_get_bit(bv, i));
    }
    putchar('\n');
}

void test_length_compression()
{
    printf("*** TEST LENGTH COMPRESSION ***\n");

    int lengths[] = {10,12,20,256,258};

    for (int i = 0; i < sizeof(lengths)/sizeof(lengths[0]); i++) {
        Bit_Vec *bv = Bit_Vec_create();

        printf("%d -> ", lengths[i]);

        Huffman_get_length_code(lengths[i], bv);
        print_bit_vec(bv);

        Bit_Vec_destroy(bv);
        free(bv);
    }
    putchar('\n');
}

void test_distance_compression()
{
    printf("*** TEST DISTANCE COMPRESSION ***\n");

    int distances[] = {6,21,8195};

    for (int i = 0; i < sizeof(distances)/sizeof(distances[0]); i++) {
        Bit_Vec *bv = Bit_Vec_create();

        printf("%d -> ", distances[i]);

        Huffman_get_length_code(distances[i], bv);
        print_bit_vec(bv);

        Bit_Vec_destroy(bv);
        free(bv);
    }
    putchar('\n');
}

int main(int argc, const char *argv[])
{
    test_length_compression();
    test_distance_compression();

    return 0;
}
