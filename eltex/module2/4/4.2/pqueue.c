#include "pqueue.h"

PriorityQueue* createQueue()
{
    PriorityQueue* queue = malloc(sizeof(PriorityQueue));
    queue->head = NULL;
    queue->size = 0;
    return queue;
}

void enqueue(PriorityQueue* queue, int data, unsigned char priority) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->priority = priority;
    newNode->next = NULL;

    if (queue->head == NULL || queue->head->priority > priority) {
        newNode->next = queue->head;
        queue->head = newNode;
    } else {
        Node* current = queue->head;
        while (current->next != NULL && current->next->priority <= priority) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    ++queue->size;
    return;
}

int dequeue(PriorityQueue* queue) {
    if (queue->head == NULL) {
        printf("Очередь пуста!\n");
        return INT_MIN;
    }

    Node* temp = queue->head;
    int data = temp->data;
    queue->head = queue->head->next;
    free(temp);
    return data;
}

int dequeuePriority(PriorityQueue* queue, unsigned char priority) {
    if (queue->head == NULL) {
        printf("Очередь пуста!\n");
        return INT_MIN;
    }

    Node* current = queue->head;
    Node* prev = NULL;

    while (current != NULL) {
        if (current->priority == priority) {
            int data = current->data;
            if (prev == NULL)
                queue->head = current->next;
            else
                prev->next = current->next;
            free(current);
            return data;
        }
        prev = current;
        current = current->next;
    }

    printf("Элемент с приоритетом %d не найден!\n", priority);
    return INT_MIN;
}

int dequeueMore(PriorityQueue* queue, unsigned char minPriority) {
    if (queue->head == NULL) {
        printf("Очередь пуста!\n");
        return INT_MIN;
    }

    Node* current = queue->head;
    Node* prev = NULL;

    while (current != NULL) {
        if (current->priority >= minPriority) {
            int data = current->data;
            if (prev == NULL)
                queue->head = current->next;
            else
                prev->next = current->next;
                
            free(current);
            return data;
        }
        prev = current;
        current = current->next;
    }

    printf("Элементы с приоритетом не ниже %d не найдены!\n", minPriority);
    return INT_MIN;
}

void closeQ(PriorityQueue* queue) {
    Node* current = queue->head;
    while (current != NULL)
    {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(queue);
    
    return;
}

