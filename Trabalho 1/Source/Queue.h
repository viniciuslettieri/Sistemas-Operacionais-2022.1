#ifndef QUEUE_H
#define QUEUE_H

#include "Util.h"
#include "Processo.h"

typedef struct _ElementoFila {
    Processo* processo;
    struct _ElementoFila* prox;
} ElementoFila;

typedef struct _Fila {
    int tam;
    ElementoFila *frente, *verso;
} Fila;

// Cria um novo elemento da Fila
ElementoFila* novoElemento(Processo* processo);

// Cria e inicializa uma nova Fila vazia
Fila* criaFila();

// Exclui a fila da memória
void excluiFila(Fila* fila);

// Adiciona um novo elemento no fim da Fila
void insereVerso(Fila* fila, Processo* processo);

// Remove um elemento do inicio da Fila
Processo* removeFrente(Fila* fila);

// Obtem a quantidade de elementos na Fila
int tam(Fila* fila);

// Obtem valor do elemento na frente da Fila
Processo* frente(Fila* fila);

// Obtem valor do elemento no fim da Fila
Processo* verso(Fila* fila);

#endif