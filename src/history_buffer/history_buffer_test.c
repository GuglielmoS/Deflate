#include <stdio.h>

#include "history_buffer.h"

int main(int argc, char *argv[])
{
    History_Buffer *hb = History_Buffer_new();

    if (hb == NULL) {
        fprintf(stderr, "[ERROR-History_Buffer_create] malloc failed!\n");
        return 1;
    }

    for (uint8_t i = 0; i < 20; i++) {
        History_Buffer_add(hb, i);

        printf("BUFFER: ");
        for (size_t j = 0; j < HISTORY_BUFFER_SIZE; j++) {
            printf("%d ", History_Buffer_get(hb, j));
        }
        putchar('\n');
    }

    History_Buffer_destroy(hb);

    return 0;
}
