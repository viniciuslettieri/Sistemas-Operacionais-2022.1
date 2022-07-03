#include <stdlib.h>
#include <stdio.h>

#include "Lista.h"

// Cria uma referencia para nova lista
Lista* criaLista() {
    Lista* lista = (Lista*) malloc(sizeof(Lista));
    lista->primeiro = NULL;
    lista->size = 0;
    return lista;
}

// Cria um elemento de lista
ListaElemento* criaElemento(int pagina) {
    ListaElemento* elemento = (ListaElemento*) malloc(sizeof(ListaElemento));
    elemento->pagina = pagina;
    elemento->proximo = NULL;
    return elemento;
}

// Busca um elemento da lista
ListaElemento* buscaElemento(Lista* lista, int pagina) {
    ListaElemento* p = lista->primeiro;
    while (p->pagina != pagina) p = p->proximo;
    
    return p;
}

// Insere um elemento de lista
void insere(Lista** lista, ListaElemento* elemento) {
    if (possui(*lista, elemento)) {
        moveElementoParaOFinal(lista, elemento);
    } else {
        if ((*lista)->primeiro == NULL) {
            (*lista)->primeiro = elemento;
        } else {
            ListaElemento* p = (*lista)->primeiro;
            while (p->proximo != NULL) {
                p = p->proximo;
            }
            p->proximo = elemento;
        }

        (*lista)->size++;
    }
}

// Remove um elemento de lista
void removeElemento(Lista** lista, ListaElemento* elemento) {
    if (possui(*lista, elemento)) {
        ListaElemento* p = (*lista)->primeiro;
        if (p == elemento) {
            (*lista)->primeiro = p->proximo;
            free(p);
        } else {
            ListaElemento* q = p->proximo;
            while (q != elemento) {
                p = p->proximo;
                q = q->proximo;
            }
            p->proximo = q->proximo;
            free(q);
        }
        (*lista)->size--;
    }
}

void imprimeLista(Lista* lista) {
    ListaElemento* p = lista->primeiro;
    while (p != NULL) {
        printf("%d\n", p->pagina);
        p = p->proximo;
    }
    printf("\n%d\n", lista->size);
}

// Verifica se lista possui elemento
int possui(Lista* lista, ListaElemento* elemento) {
    int listaPossuiElemento = 0;
    ListaElemento* p = lista->primeiro;
    if (p == NULL) return 0;
    while (p->proximo != NULL) {
        if (p == elemento) listaPossuiElemento = 1;
        p = p->proximo;
    }

    if (p == elemento) listaPossuiElemento = 1;

    return listaPossuiElemento;
}

// Assumindo que o elemento está na lista, move-o para o final dela
void moveElementoParaOFinal(Lista** lista, ListaElemento* elemento) {
    // Se o elemento já não estiver no final
    if (elemento->proximo != NULL) {
        ListaElemento* p = (*lista)->primeiro;
        ListaElemento* q = p->proximo;
      
        // Se o primeiro elemento for o desejado
        if (p == elemento) {
            (*lista)->primeiro = q;
            while (q->proximo != NULL) q = q->proximo;
            q->proximo = p;
        } else {
            while (q != elemento) {
                p = p->proximo;
                q = q->proximo;
            }
            p->proximo = q->proximo;
            while (q->proximo != NULL) {
                q = q->proximo;
            }
            q->proximo = elemento;
        }
    }

    elemento->proximo = NULL;
}