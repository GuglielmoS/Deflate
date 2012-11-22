#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "history_buffer.h"

/**
 * Allocates and returns a new History Buffer.
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
        // setting the start position to 0
        hb->next_pos = 0;
        hb->start_pos = 0;
    }

    return hb;
}

/**
 * Deallocates the History Buffer.
 */
void History_Buffer_destroy(History_Buffer *hb)
{
    free(hb);
}

/**
 * Adds 'byte' to the buffer 'hb'.
 */
void History_Buffer_add(History_Buffer *hb, uint8_t byte)
{
    if (hb->next_pos == HISTORY_BUFFER_SIZE) {
        hb->next_pos = 0;
        hb->start_pos++;
    }
    else {
        if (hb->start_pos >  0)                   hb->start_pos++;
        if (hb->start_pos == HISTORY_BUFFER_SIZE) hb->start_pos = 0;
    }

    hb->buf[hb->next_pos++] = byte;
}

/**
 * Returns the byte at the position 'index'.
 */
uint8_t History_Buffer_get(History_Buffer *hb, size_t index)
{
    return hb->buf[(hb->start_pos + index) % HISTORY_BUFFER_SIZE];
}


/**
 * History_Buffer_Context functions.
 */
void History_Buffer_Context_save(History_Buffer *hb, History_Buffer_Context *c)
{
    if (hb != NULL && c != NULL) {
        c->next_pos  = hb->next_pos;
        c->start_pos = hb->start_pos;
    }
}

void History_Buffer_Context_restore(History_Buffer *hb, History_Buffer_Context *c)
{
    if (hb != NULL && c != NULL) {
        hb->next_pos  = c->next_pos;
        hb->start_pos = c->start_pos;
    }
}
