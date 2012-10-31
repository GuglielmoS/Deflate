#include <stdlib.h>
#include <string.h>

#include "history_buffer.h"

void History_Buffer_create(History_Buffer **hb)
{
    *hb = (History_Buffer*)malloc(sizeof(History_Buffer));

    // setting to zero the buffer content
    memset((*hb)->buf, 0x00, HISTORY_BUFFER_SIZE);
    // setting the stat position to 0
    (*hb)->next_pos = 0;
}

/**
 * Add 'byte' to the buffer 'ht'.
 */
void History_Buffer_add(History_Buffer *hb, uint8_t byte)
{
    hb->buf[hb->next_pos] = byte;
    hb->next_pos = (hb->next_pos + 1) % HISTORY_BUFFER_SIZE;
}

/**
 * Returns the byte at the position 'index'.
 * ATTENTION: it doesn't check if index is greater than the
 *            size of the buffer.
 */
uint8_t History_Buffer_get(History_Buffer *hb, size_t index)
{
    return hb->buf[index];
}
