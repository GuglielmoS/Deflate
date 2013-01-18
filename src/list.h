#ifndef __LIST__
#define __LIST__

#include <stdlib.h>

#include "util.h"

// List value's type
typedef size_t List_Value;

// represents a single linked list
typedef struct __list_node {
    List_Value value;
    struct __list_node *next;
} List_Node;
typedef List_Node* List;

// represents a List which can grow only at maximum size
typedef struct {
    List first;         // pointer to the first element in the list
    size_t max_size;    // maximum number of elements
    size_t cur_size;    // current number of elements
} Limited_List;

void Limited_List_init(Limited_List* l, size_t max_size);
void Limited_List_add(Limited_List *l, List_Value value);
void Limited_List_destroy(Limited_List *l);
void List_destroy(List *l);

#endif /* __LIST__ */
