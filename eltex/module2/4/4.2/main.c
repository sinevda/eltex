#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "pqueue.h"

#define TESTNUMBER 100


int main()
{
    PriorityQueue* queue = createQueue();
    if (queue != NULL)
    {
        printf("Очередь создана.\n");
    }

    for(int i = 0; i < TESTNUMBER; ++i)
    {
        int val = rand();
        u_char pr = rand() % 255;
        printf("Добавление элемента. Значение: %d, приоритет: %hhu.\n", val, pr);
        enqueue(queue, val, pr);
    }
    
    printf("\n");

    for(int i = 0; i <= TESTNUMBER / 5; ++i)
    {
        char c[10];
        memset(c, '\0', 10);
        int i = dequeue(queue);
        if(i != INT_MIN) sprintf(c, "%d", i);
        printf("Извлечение первого элемента: %s\n", c );        
    }
    
    printf("\n");

    for(int i = 0; i <= TESTNUMBER / 3; ++i)
    {
        u_char pr = rand() % 255;
        char c[10];
        memset(c, '\0', 10);
        int i = dequeuePriority(queue, pr);
        if(i != INT_MIN) sprintf(c, "%d", i);
        printf("Извлечение элемента с приоритетом %d: %s\n", pr, c);
    }
    
    printf("\n");

    for(int i = 0; i <= TESTNUMBER / 3; ++i)
    {
        u_char pr = rand() % 255;
        char c[10];
        memset(c, '\0', 10);
        int i = dequeueMore(queue, pr);
        if(i != INT_MIN) sprintf(c, "%d", i);
        printf("Извлечение элемента с не менее %d: %s\n", pr, c);
    }

    closeQ(queue);
    return 0;
}
