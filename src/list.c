#include "list.h"

/**
 * Initialize the Limited_List 'l'.
 */
void Limited_List_init(Limited_List* l, size_t max_size)
{
    l->first = NULL;
    l->last = NULL;
    l->cur_size = 0;
    l->max_size = max_size;
}

/**
 * Adds an element to 'l'.
 */
void Limited_List_add(Limited_List *l, List_Value value)
{
    // if the list is full
    if (l->cur_size == l->max_size) {
        l->last->value = value;
    }
    else {
        List tmp_el = (List)malloc(sizeof(List_Node));
        if (tmp_el == NULL) {
            die_error("[ERROR-Limited_List_add] malloc failed!\n");
        }
        tmp_el->value = value;
        tmp_el->next = NULL;
        l->cur_size++;

        if (l->first == NULL) {
            l->first = tmp_el;
            l->last = tmp_el;
        }
        else {
            tmp_el->next = l->first;
            l->first = tmp_el;
        }
    }
}

/**
 * Deallocates the list identified by *l.
 */
void Limited_List_destroy(Limited_List *l)
{
    if (l != NULL) {
        List_destroy(&(l->first));
        l->cur_size = 0;
        l->first = NULL;
        l->last = NULL;
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

