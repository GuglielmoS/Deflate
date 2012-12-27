#ifndef __LZ_TYPES__
#define __LZ_TYPES__

#include <stdint.h>
#include <stdbool.h>

typedef enum {_LZ_LITERAL, // literal value
              _LZ_PAIR}    // distance & length pair
        LZ_Type;

// literal symbols
typedef uint8_t LZ_Literal;

// distance values
typedef uint16_t LZ_Distance;

// length values
typedef uint16_t LZ_Length;

// pair of distance and length
typedef struct {
    LZ_Distance distance;
    LZ_Length   length;
} LZ_Pair;

// value of an element; it can contains a literal
// or a distance/length pair
typedef union {
    LZ_Literal l;
    LZ_Pair p;
} LZ_Value;

#endif /* __LZ_TYPES__ */
