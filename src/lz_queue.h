#ifndef __LZ_QUEUE__
#define __LZ_QUEUE__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "util.h"
#include "lz_element.h"

// abstraction used to write the code in a more
// comprehensible way
typedef LZ_Element* LZ_Queue_Value;

// double linked list definition
typedef struct __lz_queue_node {
    LZ_Queue_Value          value; // value of the node
    struct __lz_queue_node *prev;  // pointer to the next node
    struct __lz_queue_node *next;  // pointer to the previous node
} LZ_Queue_Node;

// queue data structure definition
typedef struct {
    LZ_Queue_Node *head; // pointer to the first element
    LZ_Queue_Node *tail; // pointer to the last element
} LZ_Queue;

/*** USEFUL MACROS ***/

// allocates and enqueues a LZ_Element for the literal value 'lit'
#define LZQ_ENQUEUE_LITERAL(queue,lit)   LZ_Queue_enqueue(queue,LZ_Literal_new((lit)))

// allocates and enqueues a LZ_Element for the pair value composed by (dist,len)
#define LZQ_ENQUEUE_PAIR(queue,dist,len) LZ_Queue_enqueue(queue,LZ_Pair_new((LZ_Pair){(dist),(len)}))

// dequeues and returns the element on the top of the queue 'queue'
#define LZQ_DEQUEUE(queue)               LZ_Queue_dequeue(queue)

// true if 'queue' is empty, false otherwise
#define LZQ_IS_EMPTY(queue)              ((queue)->head == NULL)

void LZ_Queue_init(LZ_Queue *q);
void LZ_Queue_enqueue(LZ_Queue *q, LZ_Queue_Value value);
LZ_Queue_Value LZ_Queue_dequeue(LZ_Queue *q);

#endif /* __LZ_QUEUE__ */
