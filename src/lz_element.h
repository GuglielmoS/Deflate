#ifndef __LZ_ELEMENT__
#define __LZ_ELEMENT__

#include <stdio.h>
#include <stdlib.h>

#include "lz_types.h"

/**
 * defines a container of LZ77 values, thus it can 
 * represents a literal value or a distance/length pair.
 *
 * In order to known what kind of value it assumes, there is 
 * a field called 'type' that states if the element is a pair 
 * or a literal.
 */
typedef struct __lz_element {
    // attributes
    LZ_Value value;
    LZ_Type  type;
} LZ_Element;

/*** USEFUL MACROS ***/
#define LZE_GET_LITERAL(e)  (e)->value.l
#define LZE_GET_DISTANCE(e) (e)->value.p.distance
#define LZE_GET_LENGTH(e)   (e)->value.p.length
#define LZE_IS_LITERAL(e)   ((e)->type == _LZ_LITERAL)
#define LZE_IS_PAIR(e)      ((e)->type == _LZ_PAIR)

/*** CONSTRUCTORS **/
LZ_Element* LZ_Element_new();
LZ_Element* LZ_Pair_new(LZ_Pair pair);
LZ_Element* LZ_Literal_new(LZ_Literal literal);

// GETTERS
LZ_Type     LZ_Element_get_type(const LZ_Element *element);
LZ_Literal  LZ_Element_get_literal(const LZ_Element *element);
LZ_Pair     LZ_Element_get_pair(const LZ_Element *element);
LZ_Distance LZ_Element_get_distance(const LZ_Element *element);
LZ_Length   LZ_Element_get_length(const LZ_Element *element);

// SETTERS
void LZ_Element_set_type(LZ_Element *element, LZ_Type type);
void LZ_Element_set_pair_type(LZ_Element *element);
void LZ_Element_set_literal_type(LZ_Element *element);
void LZ_Element_set_literal(LZ_Element *element, LZ_Literal literal);
void LZ_Element_set_pair(LZ_Element *element, LZ_Pair pair);
void LZ_Element_set_distance(LZ_Element *element, LZ_Distance distance);
void LZ_Element_set_length(LZ_Element *element, LZ_Length length);

// OTHERS
bool LZ_Element_is_pair(const LZ_Element *element);
bool LZ_Element_is_literal(const LZ_Element *element);
void LZ_Element_copy_to(const LZ_Element *src, LZ_Element *dest);

#endif /* __LZ_ELEMENT__ */
