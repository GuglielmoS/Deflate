#ifndef __LIST__
#define __LIST__

#include <stdlib.h>
#include <stddef.h>

typedef size_t List_Value;

typedef struct __list_node {
    List_Value value;
    struct __list_node *next;
} List_Node;
typedef List_Node* List;

void List_create(List *l, List_Value init_val);
void List_add(List *l, List_Value val);
void List_keep_only_last_n_values(List *l, size_t n);
void List_destroy(List *l);

#endif /* __LIST__ */
