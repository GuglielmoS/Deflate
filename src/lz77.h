#ifndef __LZ77__
#define __LZ77__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// max buffer size
#define BUFFER_SIZE 32768

/*** DATA STRUCTURES ***/

typedef enum {_LZ_LITERAL, // literal value
              _LZ_PAIR}    // distance & length pair
        LZ_Type;

// literal symbols
typedef int8_t LZ_Literal;

// distance values
typedef int16_t LZ_Distance;

// length values
typedef int16_t LZ_Length;

// pair of distance and length
typedef struct {
    LZ_Distance distance;
    LZ_Length   length;
} LZ_Pair;

// value of an LZ_Element, it can contains a literal
// or a distance/length pair
typedef union {
    LZ_Literal l;
    LZ_Pair p;
} LZ_Value;

// defines a container of LZ77 values, thus it can 
// represents a literal value or a distance/length pair.
//
// In order to known what kind of value it assumes, there is 
// a field called 'type' that states if the element is a pair 
// or a literal.
typedef struct {
    LZ_Value value;
    LZ_Type  type;
} LZ_Element;

// represents a stream of 'size' LZ_Element values 
typedef struct {
    LZ_Element *values;
    size_t      size;
} LZ_Stream;


/*** DATA STRUCTURES ACCESSORS ***/

/**
 * Constructors of LZ_Element.
 * Allocates and returns a new LZ_Element.
 */
LZ_Element *LZ_new_element();

/**
 * Returns the type of the element.
 */
LZ_Type LZ_get_element_type(LZ_Element *element);

/**
 * Returns the literal value of element.
 */
LZ_Literal LZ_get_literal(const LZ_Element *element);

/**
 * Returns the pair value of element.
 */
LZ_Pair LZ_get_pair(const LZ_Element *element);

/**
 * Returns the distance value stored in the pair field
 * of element.
 */
LZ_Distance LZ_get_distance(const LZ_Element *element);

/**
 * Returns the length value stored in the pair field
 * of element.
 */
LZ_Length LZ_get_length(const LZ_Element *element);

/**
 * Sets the type field of element to the 'type' parameter value.
 */
void LZ_set_type(LZ_Element *element, LZ_Type type);

/**
 * Changes the type of element so that it can be seen
 * as a pair.
 */
void LZ_set_pair_type(LZ_Element *element);

/**
 * Changes the type of element so that it can be seen
 * as a literal.
 */
void LZ_set_literal_type(LZ_Element *element);

/**
 * Sets the litearl value of element to the 'literal' parameter value.
 */
void LZ_set_literal(LZ_Element *element, LZ_Literal literal);

/**
 * Sets the pair value of element to the 'pair' parameter value.
 */
void LZ_set_pair(LZ_Element *element, LZ_Pair pair);

/**
 * Sets the distance value of the pair field of element.
 */
void LZ_set_distance(LZ_Element *element, LZ_Distance distance);

/**
 * Sets the length value of the pair field of element.
 */
void LZ_set_length(LZ_Element *element, LZ_Length length);

/**
 * Sets the 'dest' element to the values of 'src'.
 */
void LZ_copy_element(const LZ_Element *src, LZ_Element *dest);

/**
 * Returns true if element is a literal, false otherwise.
 */
bool LZ_is_literal(const LZ_Element* element);

/**
 * Returns true if element is a distance/length pair, false otherwise.
 */
bool LZ_is_pair(const LZ_Element *element);
 
/**
 * Allocates and return a new LZ_Stream of 0 elements.
 */
LZ_Stream* LZ_new_stream();

/**
 * Appends 'element' to 'stream'.
 */
void LZ_stream_append(LZ_Stream *stream, const LZ_Element *element);

/**
 * Returns the LZ_Element stored in the stream at position 'pos'. 
 */
LZ_Element* LZ_stream_get(LZ_Stream *stream, size_t pos);

/**
 * Sets the LZ_Element element at the position 'pos' in 'stream' to
 * the values of 'element'.
 */
void LZ_stream_set(LZ_Stream *stream, LZ_Element *element, size_t pos);

/**
 * Returns the size of 'stream'.
 */
size_t LZ_stream_size(LZ_Stream *stream);

/**
 * Increments the size of stream.
 */
void LZ_stream_inc_size(LZ_Stream *stream);

/**
 * Prints to the FILE descriptor passed as argoument
 * a nice representation of the LZ_stream 'stream'.
 */
void LZ_stream_pretty_print(LZ_Stream *stream, FILE *fd);

/*** LZ77 INTERFACE FUNCTIONS (API) ***/

/**
 * Returns a stream of LZ_Element that represents the
 * LZ77 encoding of 'buffer'.
 */
LZ_Stream* LZ_encode(const char *buffer); 

/**
 * Returns an array of characters by decoding the 
 * LZ_Element array.
 */
char* LZ_decode(LZ_Stream *stream);

#endif /* __LZ77__ */

