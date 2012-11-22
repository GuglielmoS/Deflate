#ifndef __LZ_ELEMENT__
#define __LZ_ELEMENT__

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

    // methods
    LZ_Type     (*get_type)(const struct __lz_element *);
    LZ_Literal  (*get_literal)(const struct __lz_element *);
    LZ_Pair     (*get_pair)(const struct __lz_element *);
    LZ_Distance (*get_distance)(const struct __lz_element *);
    LZ_Length   (*get_length)(const struct __lz_element *);
    void        (*set_type)(struct __lz_element *, LZ_Type);
    void        (*set_pair_type)(struct __lz_element *);
    void        (*set_literal_type)(struct __lz_element *);
    void        (*set_literal)(struct __lz_element *, LZ_Literal);
    void        (*set_pair)(struct __lz_element *, LZ_Pair);
    void        (*set_distance)(struct __lz_element *, LZ_Distance);
    void        (*set_length)(struct __lz_element *, LZ_Length);
    bool        (*is_pair)(const struct __lz_element *);
    bool        (*is_literal)(const struct __lz_element *);
    void        (*copy_to)(const struct __lz_element *, struct __lz_element  *);
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
