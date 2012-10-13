#include <stdio.h>
#include <stdlib.h>

#include "lz_stream.h"

LZ_Stream* LZ_Stream_new()
{
    LZ_Stream *s = (LZ_Stream*)malloc(sizeof(LZ_Stream));

    if (s == NULL) {
        fprintf(stderr, "[ERROR-LZ_Stream_new] malloc failed!");
        exit(EXIT_FAILURE);
    }

    s->__s = Stream_new();
    
    s->append   = LZ_Stream_append;
    s->get      = LZ_Stream_get;
    s->set      = LZ_Stream_set;
    s->get_size = LZ_Stream_get_size;
    s->is_full  = LZ_Stream_is_full;

    return s;
}

void LZ_Stream_append(LZ_Stream *stream, const LZ_Element *element)
{
    //(stream->__s)->append(stream->__s, element);
    Stream_append(stream->__s, (void*)element);
}

void LZ_Stream_set(LZ_Stream *stream, const LZ_Element *element, size_t pos)
{
    //(stream->__s)->set(stream->__s, element, pos);
    Stream_set(stream->__s, (void*)element, pos);
}

LZ_Element* LZ_Stream_get(const LZ_Stream *stream, size_t pos)
{
    //return (LZ_Element*)((stream->__s)->get(stream->__s, pos));
    return (LZ_Element*)(Stream_get(stream->__s, pos));
}

size_t LZ_Stream_get_size(const LZ_Stream *stream)
{
    //return (stream->__s)->get_size(stream->__s);
    return Stream_get_size(stream->__s);
}

bool LZ_Stream_is_full(const LZ_Stream *stream)
{
    //return (stream->__s)->is_full(stream->__s);
    return Stream_is_full(stream->__s);
}

void LZ_Stream_destroy(LZ_Stream *stream)
{
    for (size_t i = 0; i < Stream_get_size(stream->__s); i++) {
        free((LZ_Element*)Stream_get(stream->__s, i));
    }
    Stream_destroy(stream->__s);
}

/**
 * Prints to the FILE descriptor passed as argoument
 * a representation of the LZ_stream 'stream'.
 */
void LZ_Stream_print(LZ_Stream *s, FILE *fd)
{
    if (s != NULL) {
        for (size_t i = 0; i < s->get_size(s); i++) {
            LZ_Element *cur_el = s->get(s, i);
        
            if (cur_el->is_literal(cur_el)) {
                fprintf(fd, "%c", cur_el->get_literal(cur_el));
            }
            else {
                fprintf(fd, "[D=%d, L=%d]", cur_el->get_distance(cur_el),
                                            cur_el->get_length(cur_el));
            }
        }

        fprintf(fd, "\n");
    }
}

