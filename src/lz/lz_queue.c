#include "lz_queue.h"

#include <stdio.h>
#include <stdlib.h>

LZ_Queue_Node* LZ_Queue_Node_new(LZ_Queue_Value value)
{
    LZ_Queue_Node *new_node = (LZ_Queue_Node*)malloc(sizeof(LZ_Queue_Node));

    if (new_node == NULL) {
        fprintf(stderr, "[ERROR-LZ_Queue_Node_new] malloc failed!\n");
        exit(EXIT_FAILURE);
    }
    else {
        new_node->value = value;
        new_node->next  = NULL;
    }

    return new_node;
}

void LZ_Queue_init(LZ_Queue *q)
{
    q->head = NULL;
    q->tail = NULL;
}

void LZ_Queue_enqueue(LZ_Queue *q, LZ_Queue_Value value)
{
    LZ_Queue_Node *new_node = LZ_Queue_Node_new(value);

    if (q->head == q->tail) {
        // empty queue
        if (q->head == NULL) {
            q->tail = new_node;
            q->head = q->tail;
        }
        // one element queue
        else {
            q->tail->next = new_node;
            q->tail       = new_node;
        }
    }
    // filled queue
    else {
        q->tail->next = new_node;
        q->tail       = new_node;
    }
}

LZ_Queue_Value LZ_Queue_dequeue(LZ_Queue *q)
{
    LZ_Queue_Value removed_value;
    if (q->head == q->tail) {
        if (q->head != NULL) {
            removed_value = q->head->value;
            q->head = NULL;
            q->tail = q->head;
        }
    }
    else {
        removed_value = q->head->value;
        q->head = q->head->next;
    }

    return removed_value;
}

bool LZ_Queue_is_empty(LZ_Queue *q)
{
    return q->head == NULL && q->tail == NULL;
}
