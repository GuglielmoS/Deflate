#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lz77.h"

/**
 * Constructors of LZ_Element.
 * Allocates and returns a new LZ_Element.
 */
LZ_Element *LZ_new_element()
{
    LZ_Element *element = (LZ_Element*)malloc(sizeof(LZ_Element));
    
    if (element == NULL) {
        fprintf(stderr, "[ERROR] malloc on LZ_Element failed!\n");
        exit(EXIT_FAILURE);
    }

    return element;
}

/**
 * Returns the type of the element.
 */
LZ_Type LZ_get_element_type(LZ_Element *element)
{
    return element->type;
}

/**
 * Returns the literal value of element.
 */
LZ_Literal LZ_get_literal(const LZ_Element *element)
{
    return element->value.l;
}

/**
 * Returns the pair value of element.
 */
LZ_Pair LZ_get_pair(const LZ_Element *element) 
{
    return element->value.p;
}

/**
 * Returns the distance value stored in the pair field
 * of element.
 */
LZ_Distance LZ_get_distance(const LZ_Element *element) 
{
    return LZ_get_pair(element).distance;
}

/**
 * Returns the length value stored in the pair field
 * of element.
 */
LZ_Length LZ_get_length(const LZ_Element *element) 
{
    return LZ_get_pair(element).length;
} 

/**
 * Sets the type field of element to the 'type' parameter value.
 */
void LZ_set_type(LZ_Element *element, LZ_Type type) 
{
    element->type = type;
}

/**
 * Changes the type of element so that it can be seen
 * as a pair.
 */
void LZ_set_pair_type(LZ_Element *element)
{
    LZ_set_type(element, _LZ_PAIR);
}

/**
 * Changes the type of element so that it can be seen
 * as a literal.
 */
void LZ_set_literal_type(LZ_Element *element)
{
    LZ_set_type(element, _LZ_LITERAL);
}

/**
 * Sets the litearl value of element to the 'literal' parameter value.
 */
void LZ_set_literal(LZ_Element *element, LZ_Literal literal) 
{
    LZ_set_literal_type(element);
    element->value.l = literal;
}

/**
 * Sets the pair value of element to the 'pair' parameter value.
 */
void LZ_set_pair(LZ_Element *element, LZ_Pair pair) 
{
    LZ_set_pair_type(element);
    element->value.p.distance = pair.distance;
    element->value.p.length = pair.length;
}

/**
 * Sets the distance value of the pair field of element.
 */
void LZ_set_distance(LZ_Element *element, LZ_Distance distance) 
{
    element->value.p.distance = distance;
}

/**
 * Sets the length value of the pair field of element.
 */
void LZ_set_length(LZ_Element *element, LZ_Length length)
{
    element->value.p.length = length;
}

/**
 * Sets the 'dest' element to the values of 'src'.
 */
void LZ_copy_element(const LZ_Element *src, LZ_Element *dest)
{
    dest->value.l = src->value.l;
    dest->value.p = src->value.p;
    dest->type    = src->type;
}

/**
 * Returns true if element is a literal, false otherwise.
 */
bool LZ_is_literal(const LZ_Element *element) 
{
    return element->type == _LZ_LITERAL;
}

/**
 * Returns true if element is a distance/length pair, false otherwise.
 */
bool LZ_is_pair(const LZ_Element *element)
{
    return !(LZ_is_literal(element));
}

/**
 * Allocates and returns a new LZ_Stream of 0 elements.
 */
LZ_Stream* LZ_new_stream()
{
    LZ_Stream *stream = (LZ_Stream*)malloc(sizeof(LZ_Stream));

    if (stream == NULL) {
        fprintf(stderr, "[ERROR] malloc on LZ_stream failed!\n");
        exit(EXIT_FAILURE);
    }
    else {
        stream->values = NULL;
        stream->size = 0;
    }

    return stream;
}

/**
 * Appends 'element' to 'stream'.
 * Stream should be instantiated before the use of append.
 */
void LZ_stream_append(LZ_Stream *stream, const LZ_Element *element)
{
    size_t stream_size = LZ_stream_size(stream);

    // resize the arrays of values
    if (stream_size == 0) {
        stream->values = (LZ_Element*)malloc(sizeof(LZ_Element));
    }
    else {
        stream->values = (LZ_Element*)realloc(stream->values, stream->size + 1);
    }

    if (stream == NULL) {
        fprintf(stderr, "[ERROR] LZ_stream_append failed during memory allocation!"); 
        exit(EXIT_FAILURE);
    }
    else {
        LZ_copy_element(element, &stream->values[stream_size]);
        LZ_stream_inc_size(stream);
    }
}


/**
 * Returns a pointer to LZ_Element stored in the stream at position 'pos'. 
 */
LZ_Element* LZ_stream_get(LZ_Stream *stream, size_t pos)
{
    return &(stream->values[pos]);
}

/**
 * Sets the LZ_Element element at the position 'pos' in 'stream' to
 * the values of 'element'.
 */
void LZ_stream_set(LZ_Stream *stream, LZ_Element *element, size_t pos)
{
    LZ_copy_element(element, LZ_stream_get(stream, pos));
}

/**
 * Returns the size of 'stream'.
 */
size_t LZ_stream_size(LZ_Stream *stream)
{
    return stream->size;
}

/**
 * Increments the size of stream.
 */
void LZ_stream_inc_size(LZ_Stream *stream)
{
    stream->size++;
}

/**
 * Prints to the FILE descriptor passed as argoument
 * a nice representation of the LZ_stream 'stream'.
 */
void LZ_stream_pretty_print(LZ_Stream *stream, FILE *fd)
{
    for (size_t i = 0; i < LZ_stream_size(stream); i++) {
        LZ_Element *cur_element = LZ_stream_get(stream, i);
    
        if (LZ_is_literal(cur_element)) {
            fprintf(fd, "%c", LZ_get_literal(cur_element));
        }
        else {
            fprintf(fd, "[D=%d, L=%d]", LZ_get_distance(cur_element),
                                        LZ_get_length(cur_element));
        }
    }

    fprintf(fd, "\n");
}

/**
 * Returns a stream of LZ_Element that represents the
 * LZ77 encoding of 'message'.
 */
LZ_Stream* LZ_encode(const char *buffer) 
{
    LZ_Stream *out_stream = LZ_new_stream();

    size_t i = 0;
    size_t buf_end = 0;
    size_t buffer_length = strlen(buffer);

    while (i < buffer_length) {

        size_t length = 0;
        for (size_t j = 0; j < buf_end; j++) {

            if (buffer[j] == buffer[i]) {
                
                size_t k = 0;
                while (k < buffer_length) {
                    if (buffer[j+k] == buffer[i+k]) length++;
                    else break;
                    k++;
                }
                
                if (length > 0) {
                    LZ_Element *element = LZ_new_element();
                    LZ_set_pair(element, (LZ_Pair){buf_end-j, length});
                    LZ_stream_append(out_stream, element);

                    i += length;
                    break;
                }
            }
        }
        
        if (length == 0) {
            LZ_Element *element = LZ_new_element();
            LZ_set_literal(element, (LZ_Literal)buffer[i]);
            LZ_stream_append(out_stream, element);

            i++;
        }

        buf_end++;
    }

    return out_stream;
}

/**
 * TODO: non yet implemented
 * Returns an array of characters by decoding the 
 * LZ_Stream of LZ_Element.
 */
char* LZ_decode(LZ_Stream *stream)
{
    return NULL;
}

