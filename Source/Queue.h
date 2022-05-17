#include <stdio.h>
#include <stdlib.h>
 
struct QueueElement {
    int key;
    struct QueueElement* next;
};
 
struct Queue {
    int size;
    struct QueueElement *front, *back;
};
 
// Cria um novo elemento da Fila
struct QueueElement* newElement(int val) {
    struct QueueElement* elem = (struct QueueElement*) malloc( sizeof(struct QueueElement) );
    elem->next = NULL;
    elem->key = val;
    return elem;
}
 
// Cria e inicializa uma nova Fila vazia
struct Queue* createQueue() {
    struct Queue* newQueue = (struct Queue*) malloc(sizeof(struct Queue));
    newQueue->front = newQueue->back = NULL;
    newQueue->size = 0;
    return newQueue;
}
 
// Adiciona um novo elemento no fim da Fila
void push_back(struct Queue* queue, int val) {
    struct QueueElement* elem = newElement(val);
 
    if (queue->back == NULL) {
        queue->front = queue->back = elem;
    }else{
        queue->back->next = elem;
        queue->back = elem;
    }
    queue->size += 1;
}
 
// Remove um elemento do inicio da Fila
int pop_front(struct Queue* queue) {
    if (queue->front == NULL)
        return -1;

    int front_key = queue->front->key;
 
    struct QueueElement* aux = queue->front;
    queue->front = queue->front->next;
 
    if (queue->front == NULL)
        queue->back = NULL;

    queue->size -= 1;
 
    free(aux);
    return front_key;
}

// Obtem a quantidade de elementos na Fila
int size(struct Queue* queue) {
    return queue->size;
}

// Obtem valor do elemento na frente da Fila
int front(struct Queue* queue) {
    if (size(queue) == 0)
        return -1;
    else
        return queue->front->key;
}
 
// Obtem valor do elemento no fim da Fila
int back(struct Queue* queue) {
    if (size(queue) == 0) 
        return -1;
    else
        return queue->back->key;
}