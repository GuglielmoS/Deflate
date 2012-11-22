#include <stdio.h>
#include <stdlib.h>

#include "lz_element.h"

/**
 * Constructors of LZ_Element.
 * Allocates and returns a new LZ_Element.
 */
LZ_Element* LZ_Element_new()
{
    LZ_Element *e = (LZ_Element*)malloc(sizeof(LZ_Element));

    if (e == NULL) {
        fprintf(stderr, "[ERROR-LZ_new_element] malloc on LZ_Element failed!\n");
        exit(EXIT_FAILURE);
    }

    // sets all pointers to the respective method implementation
    e->get_type         = LZ_Element_get_type;
    e->get_literal      = LZ_Element_get_literal;
    e->get_pair         = LZ_Element_get_pair;
    e->get_distance     = LZ_Element_get_distance;
    e->get_length       = LZ_Element_get_length;
    e->set_type         = LZ_Element_set_type;
    e->set_pair_type    = LZ_Element_set_pair_type;
    e->set_literal_type = LZ_Element_set_literal_type;
    e->set_literal      = LZ_Element_set_literal;
    e->set_pair         = LZ_Element_set_pair;
    e->set_distance     = LZ_Element_set_distance;
    e->set_length       = LZ_Element_set_length;
    e->is_pair          = LZ_Element_is_pair;
    e->is_literal       = LZ_Element_is_literal;
    e->copy_to          = LZ_Element_copy_to;

    return e;
}

/**
 * Allocates and returns a new pair LZ_Element which
 * has as value 'pair'.
 */
LZ_Element* LZ_Pair_new(LZ_Pair pair) 
{
    LZ_Element *e = LZ_Element_new();
    e->set_pair(e, pair); 

    return e;
}

/**
 * Allocates and returns a new literal LZ_Element which
 * has as value 'literal'.
 */
LZ_Element* LZ_Literal_new(LZ_Literal literal)
{
    LZ_Element *e = LZ_Element_new();
    e->set_literal(e, literal);

    return e;
}

/**
 * Returns the type of the element.
 */
LZ_Type LZ_Element_get_type(const LZ_Element *e)
{
    return e->type;
}

/**
 * Returns the literal value of element.
 */
LZ_Literal LZ_Element_get_literal(const LZ_Element *element)
{
    return element->value.l;
}

/**
 * Returns the pair value of element.
 */
LZ_Pair LZ_Element_get_pair(const LZ_Element *element) 
{
    return element->value.p;
}

/**
 * Returns the distance value stored in the pair field
 * of element.
 */
LZ_Distance LZ_Element_get_distance(const LZ_Element *e) 
{
    return (e->get_pair(e)).distance;
}

/**
 * Returns the length value stored in the pair field
 * of element.
 */
LZ_Length LZ_Element_get_length(const LZ_Element *e) 
{
    return (e->get_pair(e)).length;
} 

/**
 * Sets the type field of element to the 'type' parameter value.
 */
void LZ_Element_set_type(LZ_Element *e, LZ_Type type) 
{
    e->type = type;
}

/**
 * Changes the type of element so that it can be seen
 * as a pair.
 */
void LZ_Element_set_pair_type(LZ_Element *e)
{
    e->set_type(e, _LZ_PAIR);
}

/**
 * Changes the type of element so that it can be seen
 * as a literal.
 */
void LZ_Element_set_literal_type(LZ_Element *e)
{
    e->set_type(e, _LZ_LITERAL);
}

/**
 * Sets the litearl value of element to the 'literal' parameter value.
 */
void LZ_Element_set_literal(LZ_Element *e, LZ_Literal literal) 
{
    e->set_literal_type(e);
    e->value.l = literal;
}

/**
 * Sets the pair value of element to the 'pair' parameter value.
 */
void LZ_Element_set_pair(LZ_Element *e, LZ_Pair pair) 
{
    e->set_pair_type(e);
    e->value.p.distance = pair.distance;
    e->value.p.length   = pair.length;
}

/**
 * Sets the distance value of the pair field of element.
 */
void LZ_Element_set_distance(LZ_Element *e, LZ_Distance distance) 
{
    e->value.p.distance = distance;
}

/**
 * Sets the length value of the pair field of element.
 */
void LZ_Element_set_length(LZ_Element *e, LZ_Length length)
{
    e->value.p.length = length;
}

/**
 * Sets the 'dest' element to the values of 'src'.
 */
void LZ_Element_copy_to(const LZ_Element *src, LZ_Element *dest)
{
    if (src->is_literal(src)) {
        dest->set_literal(dest, src->get_literal(src));
    }
    else {
        dest->set_pair(dest, src->get_pair(src));
    }
}

/**
 * Returns true if element is a literal, false otherwise.
 */
bool LZ_Element_is_literal(const LZ_Element *e) 
{
    return e->type == _LZ_LITERAL;
}

/**
 * Returns true if element is a distance/length pair, false otherwise.
 */
bool LZ_Element_is_pair(const LZ_Element *e)
{
    return !(e->is_literal(e));
}

