#ifndef __LIMITED_LIST__
#define __LIMITED_LIST__

#include <stddef.h>
#include <stdlib.h>

// type of elements stored
typedef size_t LL_Value;

// definition of a structure that allows to store
// a maximum number of elements as circular array
typedef struct {
    // pointer to the stored data
    LL_Value *values;

    // maximum number of elements
    size_t max_size;

    // current number of elements stored
    size_t cur_size;

    // index of the first element
    size_t start_pos;

    // next index in which we can insert an element
    size_t next_pos;
} Limited_List;

/*** USEFUL MACROS ***/

// retrieves the element at the position 'p' from the Limited_List 'll'
#define LIMITED_LIST_GET(ll,p) (ll)->values[((ll)->start_pos+(p))%(ll)->max_size]

void Limited_List_init(Limited_List *ll);
void Limited_List_add(Limited_List *ll, LL_Value value);
void Limited_List_destroy(Limited_List *ll);

#endif /* __LIMITED_LIST__ */
