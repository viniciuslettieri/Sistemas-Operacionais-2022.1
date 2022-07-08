#include <stdlib.h>
#include <stdio.h>

#include "Estruturas.h"

// Cria uma referencia para nova lista
Lista* CriaLista(int tamanhoMaximo) {
    Lista* lista = (Lista*) malloc(sizeof(Lista));
    lista->primeiro = NULL;
    lista->size = 0;
    lista->tamanhoMaximo = tamanhoMaximo;
    return lista;
}

// Cria um elemento de lista
ListaElemento* CriaElemento(Lista* lista, Pagina* pagina) {
    ListaElemento* elemento = BuscaElemento(lista, pagina);
    if (elemento == NULL) {
        elemento = (ListaElemento*) malloc(sizeof(ListaElemento));
        elemento->pagina = pagina;
        elemento->proximo = NULL;
    }
    return elemento;
}

// Busca um elemento da lista
ListaElemento* BuscaElemento(Lista* lista, Pagina* pagina) {
    ListaElemento* p = lista->primeiro;
    while (p != NULL && !(p->pagina->paginaID == pagina->paginaID && p->pagina->PID == pagina->PID)) p = p->proximo;
    
    return p;
}

// Insere um elemento de lista
ListaElemento* Insere(Lista** lista, ListaElemento* elemento) {
    ListaElemento* elementoRemovido = (ListaElemento*) NULL;
    if (Possui(*lista, elemento)) {
        MoveElementoParaOFinal(lista, elemento);
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

        if ((*lista)->size == (*lista)->tamanhoMaximo) {
            elementoRemovido = RemovePrimeiro(lista);
        } else {
            (*lista)->size++; 
        }
    }

    return elementoRemovido;
}

// Remove um elemento de lista
void RemoveElemento(Lista** lista, ListaElemento* elemento) {
    if (Possui(*lista, elemento)) {
        ListaElemento* p = (*lista)->primeiro;
        if (p->pagina->paginaID == elemento->pagina->paginaID && p->pagina->PID == elemento->pagina->PID) {
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

ListaElemento* RemovePrimeiro(Lista** lista) {
    ListaElemento* p = (*lista)->primeiro;
    (*lista)->primeiro = p->proximo;
    return p;
}
void ImprimeLista(Lista* lista) {
    ListaElemento* p = lista->primeiro;
    while (p != NULL) {
        printf("(%d,%d) ", p->pagina->paginaID, p->pagina->PID);
        p = p->proximo;
    }
    printf("\n\nTamanho = %d\n\n", lista->size);
}

// Verifica se lista possui elemento
int Possui(Lista* lista, ListaElemento* elemento) {
    int listaPossuiElemento = 0;
    ListaElemento* p = lista->primeiro;
    if (p == NULL) return 0;
    while (p->proximo != NULL) {
        if (p->pagina->paginaID == elemento->pagina->paginaID && p->pagina->PID == elemento->pagina->PID) listaPossuiElemento = 1;
        p = p->proximo;
    }

    if (p->pagina->paginaID == elemento->pagina->paginaID && p->pagina->PID == elemento->pagina->PID) listaPossuiElemento = 1;

    return listaPossuiElemento;
}

// Assumindo que o elemento está na lista, move-o para o final dela
void MoveElementoParaOFinal(Lista** lista, ListaElemento* elemento) {
    // Se o elemento já não estiver no final
    if (elemento->proximo != NULL) {
        ListaElemento* p = (*lista)->primeiro;
        ListaElemento* q = p->proximo;
      
        // Se o primeiro elemento for o desejado
        if (p->pagina->paginaID == elemento->pagina->paginaID && p->pagina->PID == elemento->pagina->PID) {
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