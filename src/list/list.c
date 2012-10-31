#include "list.h"

#include <stdlib.h>

/**
 * List constructor.
 */
void List_create(List *l, List_Value init_val)
{
    List tmp = (List)malloc(sizeof(List_Node));
    if (tmp != NULL) {
        tmp->value = init_val;
        tmp->next  = NULL;
    }

    l = &tmp;
}

void List_add(List *l, List_Value val)
{
    if (l == NULL) {
        List_create(l, val);
    }
    else {
        List new_node = (List)malloc(sizeof(List_Node));
        new_node->value = val;
        new_node->next  = *l;
        *l = new_node;
    }
}
