
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct Node {
    int             data;
    unsigned char   priority;
    struct Node*    next;
} Node;

typedef struct PriorityQueue {
    Node* head;
    unsigned size;
} PriorityQueue;

PriorityQueue* createQueue();

void enqueue(PriorityQueue*, int, unsigned char);

int dequeue(PriorityQueue* queue);

int dequeuePriority(PriorityQueue*, unsigned char);

int dequeueMore(PriorityQueue*, unsigned char);

void closeQ(PriorityQueue*);
