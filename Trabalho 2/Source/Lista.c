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
        elemento->anterior = NULL;
        elemento->proximo = NULL;
    }
    return elemento;
}

// Busca um elemento da lista
ListaElemento* BuscaElemento(Lista* lista, Pagina* pagina) {
    ListaElemento* p = lista->primeiro;
    while (p != NULL && !Igual(p->pagina, pagina)) p = p->proximo;
    
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
            elemento->anterior = p;
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
        if (Igual(p->pagina, elemento->pagina)) {
            p = RemovePrimeiro(lista);
            free(p);
        } else {
            ListaElemento* q = p->proximo;
            while (q != elemento) {
                p = p->proximo;
                q = q->proximo;
            }
            p->proximo = q->proximo;
            if (q->proximo != NULL) q->proximo->anterior = p;
            free(q);
        }
        (*lista)->size--;
    }
}

ListaElemento* RemovePrimeiro(Lista** lista) {
    ListaElemento* p = (*lista)->primeiro;
    (*lista)->primeiro = p->proximo;
    (*lista)->primeiro->anterior = NULL;
    return p;
}

void ImprimeLista(Lista* lista) {
    ListaElemento* p = lista->primeiro;
    while (p != NULL) {
        if (p->anterior != NULL) printf(
            "[%d,%d]", 
            p->anterior->pagina->paginaID, 
            p->anterior->pagina->PID
        );
        else printf("[NULL]");
        printf(" x ");
        printf(
            "(%d,%d)", 
            p->pagina->paginaID, 
            p->pagina->PID
        );
        printf(" x ");
        if (p->proximo != NULL) printf(
            "[%d,%d]\n", 
            p->proximo->pagina->paginaID, 
            p->proximo->pagina->PID
        );
        else printf("[NULL]");
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
        if (Igual(p->pagina, elemento->pagina)) listaPossuiElemento = 1;
        p = p->proximo;
    }

    if (Igual(p->pagina, elemento->pagina)) listaPossuiElemento = 1;

    return listaPossuiElemento;
}

// Assumindo que o elemento está na lista, move-o para o final dela
void MoveElementoParaOFinal(Lista** lista, ListaElemento* elemento) {
    // Se o elemento já não estiver no final
    if (elemento->proximo != NULL) {
        ListaElemento* p = (*lista)->primeiro;
        ListaElemento* q = p->proximo;
      
        // Se o primeiro elemento for o desejado
        if (Igual(p->pagina, elemento->pagina)) {
            (*lista)->primeiro = q;
            while (q->proximo != NULL) q = q->proximo;
            q->proximo = p;
            p->anterior = q;
        } else {
            while (q != elemento) {
                p = p->proximo;
                q = q->proximo;
            }
            p->proximo = q->proximo;
            if (q->proximo != NULL) q->proximo->anterior = p;
            while (q->proximo != NULL) {
                q = q->proximo;
            }
            q->proximo = elemento;
            elemento->anterior = q;
        }
    }

    elemento->proximo = NULL;
}

// int main() {
//     Lista* lista = CriaLista(5);

//     Insere(&lista, CriaElemento(lista, CriaPagina(1, 10)));
//     Insere(&lista, CriaElemento(lista, CriaPagina(4, 40)));
//     Insere(&lista, CriaElemento(lista, CriaPagina(5, 50)));
//     Insere(&lista, CriaElemento(lista, CriaPagina(2, 20)));
//     Insere(&lista, CriaElemento(lista, CriaPagina(3, 30)));

//     ImprimeLista(lista);
// }