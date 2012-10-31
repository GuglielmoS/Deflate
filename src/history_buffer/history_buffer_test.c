#include <stdio.h>

#include "history_buffer.h"

int main(int argc, char *argv[])
{
    History_Buffer *ht;

    History_Buffer_create(&ht);
    if (ht == NULL) {
        fprintf(stderr, "[ERROR-History_Buffer_create] malloc failed!\n");
        return 1;
    }

    for (uint8_t i = 0; i < 20; i++) {
        History_Buffer_add(ht, i);

        printf("BUFFER: ");
        for (size_t j = 0; j < HISTORY_BUFFER_SIZE; j++) {
            printf("%d ", History_Buffer_get(ht, j));
        }
        putchar('\n');
    }


    return 0;
}
