#include "lz_queue.h"

/**
 * Initializes the queue 'q' by setting the fields 'head' and 'tail' to NULL.
 */
void LZ_Queue_init(LZ_Queue *q)
{
    q->head = NULL;
    q->tail = NULL;
}

/**
 * Enqueus the element identified by 'value' in the queue 'q'.
 */
void LZ_Queue_enqueue(LZ_Queue *q, LZ_Queue_Value value)
{
    LZ_Queue_Node *new_node = (LZ_Queue_Node*)malloc(sizeof(LZ_Queue_Node));

    if (new_node == NULL) {
        die_error("[ERROR-LZ_Queue_Node_new] malloc failed!\n");
    }
    new_node->value = value;
    new_node->prev = NULL;

    if (LZQ_IS_EMPTY(q)) {
        q->head = new_node;
        new_node->next = NULL;
    }
    else {
        new_node->next = q->tail;
        q->tail->prev = new_node;
    }

    q->tail = new_node;
}

/**
 * Dequeues an element from the queue 'q'.
 * Returns NULL if the queue is empty.
 */
LZ_Queue_Value LZ_Queue_dequeue(LZ_Queue *q)
{
    LZ_Queue_Value removed_value = NULL;

    // zero elements
    if (q->head == q->tail) {
        if (q->head != NULL) {
            removed_value = q->head->value;
            free(q->head);
            q->head = NULL;
            q->tail = NULL;
        }
    }
    // one or more elements
    else {
        removed_value = q->head->value;
        LZ_Queue_Node *new_last = q->head->prev;
        free(q->head);
        new_last->next = NULL;
        q->head = new_last;
    }

    return removed_value;
}
