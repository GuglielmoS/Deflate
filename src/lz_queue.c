#include "lz_queue.h"

LZ_Queue_Node* LZ_Queue_Node_new(LZ_Queue_Value value)
{
    LZ_Queue_Node *new_node = (LZ_Queue_Node*)malloc(sizeof(LZ_Queue_Node));

    if (new_node == NULL) {
        die_error("[ERROR-LZ_Queue_Node_new] malloc failed!\n");
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
            free(q->head);
            q->head = NULL;
            q->tail = NULL;
        }
    }
    else {
        removed_value = q->head->value;
        LZ_Queue_Node *tmp = q->head->next;
        free(q->head);
        q->head = tmp;
    }

    return removed_value;
}

bool LZ_Queue_is_empty(LZ_Queue *q)
{
    return q->head == NULL;
}

void LZ_Queue_destroy(LZ_Queue *q)
{
    if (q != NULL) {
        if (q->head == q->tail) {
            if (q->head != NULL) {
                free(q->head);
            }
        }
        else {
            while (q->head) {
                LZ_Queue_Node *tmp_node = q->head;
                q->head = q->head->next;
                free(tmp_node);
            }
        }

        q->head = NULL;
        q->tail = NULL;
    }

}
