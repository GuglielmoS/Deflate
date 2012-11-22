#include "lz_queue.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    /*** THIS TEST WOKRS ONLY WHEN LZ_Queue_Value is int ***/
    LZ_Queue q;

    LZ_Queue_init(&q);

    for (size_t i = 0; i < 10; i++) {
        LZ_Queue_enqueue(&q, i);
    }

    while (!LZ_Queue_is_empty(&q)) {
        printf("%d\n", LZ_Queue_dequeue(&q));
    }
    */

    return 0;
}
