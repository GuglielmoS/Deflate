#include <stddef.h>
#include <stdint.h>

#ifndef __HISTORY_BUFFER__
#define __HISTORY_BUFFER__

/**
 * Defines the data structure and the accessor functions
 * to a circular buffer of bytes.
 */

// 32Kb of history
#define HISTORY_BUFFER_SIZE 32768

/** History Buffer data structure **/
typedef struct {
    uint8_t buf[HISTORY_BUFFER_SIZE]; // buffer used for storing data
    size_t  next_pos;                 // position of the next byte
    size_t  start_pos;                // position of the first byte
} History_Buffer;

typedef struct {
    size_t next_pos;
    size_t start_pos;
} History_Buffer_Context;

/** History Buffer constructor **/
History_Buffer* History_Buffer_new();

/** History Buffer destructor **/
void History_Buffer_destroy(History_Buffer *hb);

/** History Buffer accessors **/
void    History_Buffer_add(History_Buffer *hb, uint8_t byte);
uint8_t History_Buffer_get(History_Buffer *hb, size_t index);
void    History_Buffer_Context_save(History_Buffer *hb, History_Buffer_Context *c);
void    History_Buffer_Context_restore(History_Buffer *hb, History_Buffer_Context *c);

#endif /* __HISTORY_BUFFER_ */
