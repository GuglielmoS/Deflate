#ifndef __STREAM__
#define __STREAM__

#include <stddef.h>
#include <stdbool.h>

#define DEFAULT_STREAM_SIZE 1

typedef void* Element;

typedef struct {
    Element *values;
    size_t   used;
    size_t   size;
} Stream;

// methods
Stream*  Stream_new();
void     Stream_append(Stream *s, const Element e);
void     Stream_set(Stream *s, const Element e, size_t pos);
Element  Stream_get(const Stream *s, size_t pos);
size_t   Stream_get_size(const Stream *s);
bool     Stream_is_full(const Stream *s);
void     Stream_destroy(Stream *s);

// helper methods
void Stream_grow_store(Stream *s);

#endif /* __STREAM__ */
