#include "pq.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

struct PriorityQueue {
    uint32_t capacity;
    Node **narray;
    uint32_t head;
    uint32_t tail;
};

//creates a priority queue with a set capacity
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    Node **narray = (Node **) malloc(capacity * sizeof(Node *));
    q->capacity = capacity;
    q->narray = narray;
    q->head = 0;
    q->tail = 0;
    return q;
}

//deletes the prioriy queue and sets the poitner to null
void pq_delete(PriorityQueue **q) {
    Node **narray = (*q)->narray;
    free(narray);
    free(*q);
    *q = NULL;
}

//checks if the priority queue is empty
bool pq_empty(PriorityQueue *q) {
    if (q->tail == q->head) {
        return true;
    } else {
        return false;
    }
}

//checks if the priority queue is full
bool pq_full(PriorityQueue *q) {
    if (q->head == q->capacity) {
        return true;
    } else {
        return false;
    }
}

//returns the size of the priority queue
uint32_t pq_size(PriorityQueue *q) {
    return q->head - q->tail;
}

//enqueues a node based on frequency
bool enqueue(PriorityQueue *q, Node *n) {
    Node **narray = q->narray;

    //if the node is empty just put the node into queue
    if (pq_empty(q)) {
        narray[q->head] = n;
        (q->head)++;
        return true;
    }

    if (pq_full(q)) {
        return false;
    } else {
        //slot in the node based on frequency where larger frequencies
        //go to the head of the queue
        uint32_t j = q->head;
        //use of insertion
        while (j > 0 && (narray[j - 1]->frequency > n->frequency)) {
            narray[j] = narray[j - 1];
            j = j - 1;
        }

        narray[j] = n;
        q->head++;
        return true;
    }
}

//dequeues a node from the queue while also returning the value
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false;
    }
    Node **narray = q->narray;

    *n = narray[(q->tail)];

    q->tail = q->tail + 1;

    return true;
}

//prints a priority queue
void pq_print(PriorityQueue *q) {
    Node **narray = q->narray;
    for (uint32_t i = q->tail; i < q->head; i++) {
        printf(
            "%dth node, symbol: %c, frequency %lu\n", i, narray[i]->symbol, narray[i]->frequency);
    }
}
