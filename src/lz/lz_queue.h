#ifndef __LZ_QUEUE__
#define __LZ_QUEUE__

#include <stdbool.h>

#include "lz_element.h"

typedef LZ_Element* LZ_Queue_Value;

typedef struct __lz_queue_node {
    LZ_Queue_Value          value; // the value of the node
    struct __lz_queue_node *next;  // the pointer to the next node
} LZ_Queue_Node;

typedef struct {
    LZ_Queue_Node *head, *tail;    // head and tail pointer of the queue
} LZ_Queue;

/*** USEFUL MACROS ***/
#define LZQ_ENQUEUE_LITERAL(queue,lit)   LZ_Queue_enqueue(&queue,LZ_Literal_new((lit)));
#define LZQ_ENQUEUE_PAIR(queue,dist,len) LZ_Queue_enqueue(&queue, LZ_Pair_new((LZ_Pair){(dist),(len)}));
#define LZQ_IS_EMPTY(queue)              ((queue)->head == NULL)

/** LZ_Queue accessors **/
void LZ_Queue_enqueue(LZ_Queue *q, LZ_Queue_Value value);
LZ_Queue_Value LZ_Queue_dequeue(LZ_Queue *q);

/** Destructors **/
void LZ_Queue_destroy(LZ_Queue *q);

/** Helpers **/
void LZ_Queue_init(LZ_Queue *q);
bool LZ_Queue_is_empty(LZ_Queue *q);

#endif /* __LZ_QUEUE__ */
