#include <stddef.h>

#ifndef __LIST__
#define __LIST__

typedef size_t List_Value;

typedef struct __list_node {
    List_Value value;
    struct __list_node *next;
} List_Node;
typedef List_Node* List;

void List_create(List *l, List_Value init_val);
void List_add(List *l, List_Value val);

#endif /* __LIST__ */
