#ifndef __LZ77__
#define __LZ77__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 32768

#define DEFAULT_STREAM_SIZE 10

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
    size_t      used;
    size_t      size;
} LZ_Stream;


/*** DATA STRUCTURES CONSTRUCTORS */

// LZ_Element
LZ_Element* LZ_new_element();
LZ_Element* LZ_new_pair(LZ_Pair pair);
LZ_Element* LZ_new_literal(LZ_Literal literal);

// LZ_Stream
LZ_Stream* LZ_new_stream();


/*** DATA STRUCTURES ACCESSORS ***/

// LZ_Element - GETTERS
LZ_Type     LZ_get_element_type(LZ_Element *element);
LZ_Literal  LZ_get_literal(const LZ_Element *element);
LZ_Pair     LZ_get_pair(const LZ_Element *element);
LZ_Distance LZ_get_distance(const LZ_Element *element);
LZ_Length   LZ_get_length(const LZ_Element *element);

// LZ_Element - SETTERS
void LZ_set_type(LZ_Element *element, LZ_Type type);
void LZ_set_pair_type(LZ_Element *element);
void LZ_set_literal_type(LZ_Element *element);
void LZ_set_literal(LZ_Element *element, LZ_Literal literal);
void LZ_set_pair(LZ_Element *element, LZ_Pair pair);
void LZ_set_distance(LZ_Element *element, LZ_Distance distance);
void LZ_set_length(LZ_Element *element, LZ_Length length);

// LZ_Element - OTHER METHODS
void LZ_copy_element(const LZ_Element *src, LZ_Element *dest);
bool LZ_is_literal(const LZ_Element* element);
bool LZ_is_pair(const LZ_Element *element);

// LZ_Stream - GETTERS
LZ_Element* LZ_stream_get(const LZ_Stream *stream, size_t pos);
size_t      LZ_stream_size(const LZ_Stream *stream);
size_t      LZ_stream_max_size(const LZ_Stream *stream);

// LZ_Stream - SETTERS
void LZ_stream_append(LZ_Stream *stream, const LZ_Element *element);
void LZ_stream_set(LZ_Stream *stream, const LZ_Element *element, size_t pos);
void LZ_stream_inc_size(LZ_Stream *stream);


/*** LZ77 INTERFACE FUNCTIONS (API) ***/

LZ_Stream* LZ_encode(const char *buffer); 
LZ_Stream* LZ_decode(const LZ_Stream *in_stream);
void       LZ_stream_print(LZ_Stream *stream, FILE *fd);

#endif /* __LZ77__ */

