#include "lz_element.h"

/**
 * Constructors of LZ_Element.
 * Allocates and returns a new LZ_Element.
 */
LZ_Element* LZ_Element_new()
{
    LZ_Element *e = (LZ_Element*)malloc(sizeof(LZ_Element));

    if (e == NULL) {
        die_error("ERROR-LZ_new_element] malloc on LZ_Element failed!\n");
    }

    return e;
}

/**
 * Allocates and returns a new pair LZ_Element which
 * has as value 'pair'.
 */
LZ_Element* LZ_Pair_new(LZ_Pair pair)
{
    LZ_Element *e = LZ_Element_new();
    LZ_Element_set_pair(e, pair);

    return e;
}

/**
 * Allocates and returns a new literal LZ_Element which
 * has as value 'literal'.
 */
LZ_Element* LZ_Literal_new(LZ_Literal literal)
{
    LZ_Element *e = LZ_Element_new();
    LZ_Element_set_literal(e, literal);

    return e;
}

/**
 * Sets the litearl value of element to the 'literal' parameter value.
 */
void LZ_Element_set_literal(LZ_Element *e, LZ_Literal literal) 
{
    e->type = _LZ_LITERAL;
    e->value.l = literal;
}

/**
 * Sets the pair value of element to the 'pair' parameter value.
 */
void LZ_Element_set_pair(LZ_Element *e, LZ_Pair pair) 
{
    e->type = _LZ_PAIR;
    e->value.p.distance = pair.distance;
    e->value.p.length   = pair.length;
}
