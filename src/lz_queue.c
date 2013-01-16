#include "lz_queue.h"

LZ_Queue_Node* LZ_Queue_Node_new(LZ_Queue_Value value)
{
    LZ_Queue_Node *new_node = (LZ_Queue_Node*)malloc(sizeof(LZ_Queue_Node));

    if (new_node == NULL) {
        die_error("[ERROR-LZ_Queue_Node_new] malloc failed!\n");
    }
    else {
        new_node->value = value;
        new_node->prev = NULL;
        new_node->next = NULL;
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
    LZ_Queue_Node *new_node = (LZ_Queue_Node*)malloc(sizeof(LZ_Queue_Node));

    if (new_node == NULL) {
        die_error("[ERROR-LZ_Queue_Node_new] malloc failed!\n");
    }
    new_node->value = value;

    // zero or one elements
    if (q->head == q->tail) {
        // zero elements
        if (q->head == NULL) {
            q->head = new_node;
            q->tail = new_node;
            new_node->next = NULL;
            new_node->prev = NULL;
        }
        // one element
        else {
            new_node->prev = NULL;
            new_node->next = q->tail;
            q->tail->prev = new_node;
            q->tail = new_node;
        }
    }
    // two or more elements
    else {
        new_node->prev = NULL;
        new_node->next = q->tail;
        q->tail->prev = new_node;
        q->tail = new_node;
    }
}

LZ_Queue_Value LZ_Queue_dequeue(LZ_Queue *q)
{
    LZ_Queue_Value removed_value;

    // zero or one elements
    if (q->head == q->tail) {
        if (q->head != NULL) {
            removed_value = q->head->value;
            free(q->head);
            q->head = NULL;
            q->tail = NULL;
        }
    }
    // two or more elements
    else {
        removed_value = q->head->value;
        LZ_Queue_Node *new_last = q->head->prev;
        free(q->head);
        new_last->next = NULL;
        q->head = new_last;
    }

    return removed_value;
}

void LZ_Queue_destroy(LZ_Queue *q)
{
    if (q != NULL) {
        if (q->head == q->tail) {
            if (q->head) {
                free(q->head);
            }
        }
        else {
            while (q->tail) {
                LZ_Queue_Node *tmp_node = q->tail;
                q->tail = q->tail->next;
                free(tmp_node);
            }
        }

        q->head = NULL;
        q->tail = NULL;
    }
}
