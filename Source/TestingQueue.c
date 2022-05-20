#include <stdio.h>
#include "Queue.h"

int main() {
    Fila* q = criaFila();

    insereVerso(q, 10);
    insereVerso(q, 20);

    printf("Queue Size: %d \n", tam(q));
    printf("Queue Front : %d \n", frente(q));
    printf("Queue Rear : %d \n", verso(q));

    removeFrente(q);
    removeFrente(q);

    printf("Queue Size: %d \n", tam(q));
    printf("Queue Front : %d \n", frente(q));
    printf("Queue Rear : %d \n", verso(q));

    insereVerso(q, 30);
    insereVerso(q, 40);
    insereVerso(q, 50);

    printf("Queue Size: %d \n", tam(q));
    printf("Queue Front : %d \n", frente(q));
    printf("Queue Rear : %d \n", verso(q));

    removeFrente(q);

    printf("Queue Size: %d \n", tam(q));
    printf("Queue Front : %d \n", frente(q));
    printf("Queue Rear : %d \n", verso(q));

    return 0;
}