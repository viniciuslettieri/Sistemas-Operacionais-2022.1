#include <stdlib.h>
#include <stdio.h>

#include "ListaIO.h"

// Cria uma referencia para nova lista
Lista* criaLista() {
    Lista* lista = (Lista*) malloc(sizeof(Lista));
    lista->primeiro = NULL;
    lista->size = 0;
    return lista;
}

// Exclui uma lista da memoria
void excluiListaIO(Lista* lista) { 
    while(removeElemento(lista, 0) != NULL) {
        continue;
    }
    free(lista); 
}

// Cria um elemento de lista
ListaElemento* criaElemento(int tmp_ent, int tmp_sai, int t_io, Processo* p) {
    ListaElemento* elemento = (ListaElemento*) malloc(sizeof(ListaElemento));
    elemento->tempo_entrada = tmp_ent;
    elemento->tempo_saida = tmp_sai;
    elemento->tipo_io = t_io;
    elemento->processo = p;
    elemento->proximo = NULL;
    return elemento;
}

// Insere um elemento de lista
void insere(Lista* lista, ListaElemento* elemento) {
    if (lista->primeiro == NULL) {
        lista->primeiro = elemento;
    } else {
        ListaElemento* p = lista->primeiro;
        while (p->proximo != NULL) {
            p = p->proximo;
        }
        p->proximo = elemento;
    }
    lista->size++;
}

// Remove um elemento de lista
ListaElemento* removeElemento(Lista* lista, ListaElemento* elemento) {
    if (lista->primeiro == NULL) return NULL;

    if (lista->primeiro->proximo == NULL) {
        if (lista->primeiro == elemento) {
            free(lista->primeiro);
            lista->primeiro = NULL;
            lista->size--;
            return elemento;
        } else
            return NULL;
    }

    if (lista->primeiro == elemento && lista->primeiro->proximo != NULL) {
        ListaElemento* prox = lista->primeiro->proximo;
        free(lista->primeiro);
        lista->primeiro = prox;
        return prox;
    }

    ListaElemento* anterior = lista->primeiro;
    ListaElemento* atual = lista->primeiro->proximo;

    while (atual != elemento && atual->proximo != NULL) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == elemento) {
        if (atual->proximo == NULL) {
            anterior->proximo = NULL;
        } else {
            anterior->proximo = atual->proximo;
        }
        lista->size--;

        free(atual);

        return elemento;
    } else
        return NULL;
}

void imprimeLista(Lista* lista) {
    ListaElemento* p = lista->primeiro;
    while (p != NULL) {
        printf("%d, %d, %d\n", p->tempo_entrada, p->tempo_saida, p->tipo_io);
        p = p->proximo;
    }
    printf("%d\n", lista->size);
}

ListaElemento* pegaPrimeiroDaLista(Lista* lista) { 
    return lista->primeiro; 
}