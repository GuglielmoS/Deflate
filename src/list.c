#include "list.h"

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

void List_keep_only_last_n_values(List *l, size_t n)
{
    List tmp_ptr = *l;

    size_t list_len = 0;
    while (tmp_ptr->next != NULL && list_len < n) {
        tmp_ptr = tmp_ptr->next;
        list_len++;
    }

    if (list_len == n) {
        List old_values_list = tmp_ptr->next;
        List_destroy(&old_values_list);
        tmp_ptr->next = NULL;
    }
}

void List_destroy(List *l)
{
    while (*l != NULL) {
        List node_to_delete = *l;
        *l = (*l)->next;
        free(node_to_delete);
    }
}

