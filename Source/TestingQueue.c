#include <stdio.h>
#include "Queue.h"

int main() {
    Queue* q = createQueue();

    push_back(q, 10);
    push_back(q, 20);

    printf("Queue Size: %d \n", size(q));
    printf("Queue Front : %d \n", front(q));
    printf("Queue Rear : %d \n", back(q));

    pop_front(q);
    pop_front(q);

    printf("Queue Size: %d \n", size(q));
    printf("Queue Front : %d \n", front(q));
    printf("Queue Rear : %d \n", back(q));

    push_back(q, 30);
    push_back(q, 40);
    push_back(q, 50);

    printf("Queue Size: %d \n", size(q));
    printf("Queue Front : %d \n", front(q));
    printf("Queue Rear : %d \n", back(q));

    pop_front(q);

    printf("Queue Size: %d \n", size(q));
    printf("Queue Front : %d \n", front(q));
    printf("Queue Rear : %d \n", back(q));

    return 0;
}