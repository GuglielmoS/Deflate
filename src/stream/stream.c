#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stream.h"

/**
 * Helper methods (private)
 */

void Stream_grow_store(Stream *s)
{
    size_t new_size = s->size * 2;
    Element *new_memory = (Element*)malloc(sizeof(Element)*new_size);

    if (new_memory == NULL) {
        fprintf(stderr, "[ERROR-Stream_new] malloc failed!");
        exit(EXIT_FAILURE);
    }

    // copy old values in the new memory area
    memcpy(new_memory, s->values, sizeof(Element)*new_size);
    
    // free old memory
    free(s->values);
    
    // set the pointer at the values to the new memory area
    s->values = new_memory;
    s->size   = new_size;
}

/*** Public methods ***/

Stream* Stream_new()
{
    Stream *stream = (Stream*)malloc(sizeof(Stream));
    if (stream == NULL) {
        fprintf(stderr, "[ERROR-Stream_new] malloc failed!");
        exit(EXIT_FAILURE);
    }

    stream->values = (Element*)malloc(sizeof(Element)*DEFAULT_STREAM_SIZE);
    if (stream->values == NULL) {
        fprintf(stderr, "[ERROR-Stream_new] malloc failed!");
        exit(EXIT_FAILURE);
    }
    
    stream->size   = DEFAULT_STREAM_SIZE;
    stream->used   = 0;

    return stream;
}

void Stream_append(Stream *s, const Element e)
{
    if (s->used == s->size) {
        Stream_grow_store(s);
    }

    s->values[s->used] = e;
    s->used++;
}

inline void Stream_set(Stream *s, const Element e, size_t pos)
{
    s->values[pos] = e;
}

inline Element Stream_get(const Stream *s, size_t pos)
{
    return s->values[pos];
}

inline size_t Stream_get_size(const Stream *s)
{
    return s->used;
}

inline bool Stream_is_full(const Stream *s)
{
    return (s->used == s->size);
}

void Stream_destroy(Stream *s)
{
    free(s);
}


