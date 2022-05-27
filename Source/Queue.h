#include <stdio.h>
#include <stdlib.h>

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
ElementoFila* novoElemento(Processo* processo) {
    ElementoFila* elem = (ElementoFila*)malloc(sizeof(ElementoFila));
    elem->prox = NULL;
    elem->processo = processo;
    return elem;
}

// Cria e inicializa uma nova Fila vazia
Fila* criaFila() {
    Fila* novaFila = (Fila*)malloc(sizeof(Fila));
    novaFila->frente = NULL;
    novaFila->verso = NULL;
    novaFila->tam = 0;
    return novaFila;
}

// Adiciona um novo elemento no fim da Fila
void insereVerso(Fila* fila, Processo* processo) {
    ElementoFila* elem = novoElemento(processo);

    if (fila->verso == NULL) {
        fila->frente = elem;
        fila->verso = elem;
    } else {
        fila->verso->prox = elem;
        fila->verso = elem;
    }
    fila->tam += 1;
}

// Remove um elemento do inicio da Fila
Processo* removeFrente(Fila* fila) {
    if (fila->frente == NULL) return NULL;

    Processo* frente_processo = fila->frente->processo;

    ElementoFila* aux = fila->frente;
    fila->frente = fila->frente->prox;

    if (fila->frente == NULL) fila->verso = NULL;

    fila->tam -= 1;
    free(aux);
    return frente_processo;
}

// Obtem a quantidade de elementos na Fila
int tam(Fila* fila) { return fila->tam; }

// Obtem valor do elemento na frente da Fila
Processo* frente(Fila* fila) {
    if (tam(fila) == 0)
        return NULL;
    else
        return fila->frente->processo;
}

// Obtem valor do elemento no fim da Fila
Processo* verso(Fila* fila) {
    if (tam(fila) == 0)
        return NULL;
    else
        return fila->verso->processo;
}