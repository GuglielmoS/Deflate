#ifndef __LZ_STREAM__
#define __LZ_STREAM__

#include <stddef.h>
#include <stdbool.h>

#include "../stream/stream.h"
#include "lz_element.h"

typedef struct __lz_stream {
    // attributes
    Stream *__s;

    // methods
    void        (*append)(struct __lz_stream *, const LZ_Element *);
    void        (*set)(struct __lz_stream *, const LZ_Element *, size_t);
    LZ_Element* (*get)(const struct __lz_stream *, size_t);
    size_t      (*get_size)(const struct __lz_stream *);
    bool        (*is_full)(const struct __lz_stream *);
} LZ_Stream;

LZ_Stream*  LZ_Stream_new();
void        LZ_Stream_append(LZ_Stream *stream, const LZ_Element *element);
void        LZ_Stream_set(LZ_Stream *stream, const LZ_Element *element, size_t pos);
LZ_Element* LZ_Stream_get(const LZ_Stream *stream, size_t pos);
size_t      LZ_Stream_get_size(const LZ_Stream *stream);
bool        LZ_Stream_is_full(const LZ_Stream *stream);
void        LZ_Stream_destroy(LZ_Stream *s);
void        LZ_Stream_print(LZ_Stream *s, FILE *fd);

#endif __CHAR_STREAM__

