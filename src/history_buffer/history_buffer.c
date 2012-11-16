#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history_buffer.h"

/**
 * Allocate and return a new History Buffer.
 */
History_Buffer* History_Buffer_new()
{
    History_Buffer *hb = (History_Buffer*)malloc(sizeof(History_Buffer));

    if (hb == NULL) {
        fprintf(stderr, "[ERROR-History_Buffer_new] malloc failed!\n");
        exit(EXIT_FAILURE);
    }
    else {
        // setting to zero the buffer content
        memset(hb->buf, 0x00, HISTORY_BUFFER_SIZE);
        // setting the stat position to 0
        hb->next_pos = 0;
        hb->start_pos = 0;
    }

    return hb;
}

/**
 * Deallocate the History Buffer.
 */
void History_Buffer_destroy(History_Buffer *hb)
{
    free(hb);
}

/**
 * Add 'byte' to the buffer 'ht'.
 */
void History_Buffer_add(History_Buffer *hb, uint8_t byte)
{
    if (hb->next_pos == HISTORY_BUFFER_SIZE) {
        hb->next_pos = 0;
        hb->start_pos++;
    }
    else {
        if (hb->start_pos > 0) hb->start_pos++;
        if (hb->start_pos == HISTORY_BUFFER_SIZE) hb->start_pos = 0;
    }

    hb->buf[hb->next_pos++] = byte;
}

/**
 * Returns the byte at the position 'index'.
 * ATTENTION: it doesn't check if index is greater than the
 *            size of the buffer.
 */
uint8_t History_Buffer_get(History_Buffer *hb, size_t index)
{
    return hb->buf[(hb->start_pos + index) % HISTORY_BUFFER_SIZE];
}
