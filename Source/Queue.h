#include <stdio.h>
#include <stdlib.h>
#include "Process.h"
 
typedef struct _QueueElement {
    Processo* processo;
    struct _QueueElement* next;
} QueueElement;
 
typedef struct _Queue {
    int size;
    QueueElement *front, *back;
} Queue;
 
// Cria um novo elemento da Fila
QueueElement* newElement(Processo* val) {
    QueueElement* elem = (QueueElement*) malloc( sizeof(QueueElement) );
    elem->next = NULL;
    elem->key = val;
    return elem;
}
 
// Cria e inicializa uma nova Fila vazia
Queue* createQueue() {
    Queue* newQueue = (Queue*) malloc( sizeof(Queue) );
    newQueue->front = NULL;
    newQueue->back = NULL;
    newQueue->size = 0;
    return newQueue;
}
 
// Adiciona um novo elemento no fim da Fila
void push_back(Queue* queue, Processo* val) {
    QueueElement* elem = newElement(val);
 
    if (queue->back == NULL) {
        queue->front = elem;
        queue->back = elem;
    }else{
        queue->back->next = elem;
        queue->back = elem;
    }
    queue->size += 1;
}
 
// Remove um elemento do inicio da Fila
Processo* pop_front(Queue* queue) {
    if (queue->front == NULL)
        return -1;

    Processo* front_key = queue->front->key;
 
    QueueElement* aux = queue->front;
    queue->front = queue->front->next;

    if (queue->front == NULL)
        queue->back = NULL;

    queue->size -= 1;
    return front_key;
}

// Obtem a quantidade de elementos na Fila
int size(Queue* queue) {
    return queue->size;
}

// Obtem valor do elemento na frente da Fila
Processo* front(Queue* queue) {
    if (size(queue) == 0)
        return -1;
    else
        return queue->front->key;
}
 
// Obtem valor do elemento no fim da Fila
Processo* back(Queue* queue) {
    if (size(queue) == 0) 
        return -1;
    else
        return queue->back->key;
}