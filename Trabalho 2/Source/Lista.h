#ifndef LISTA_H
#define LISTA_H

typedef struct _ListaElemento {
    int pagina;
    struct _ListaElemento* proximo;
} ListaElemento;

typedef struct _Lista {
    int size;
    ListaElemento* primeiro;
} Lista;

Lista* criaLista();

ListaElemento* buscaElemento(Lista* lista, int pagina);

ListaElemento* criaElemento(Lista*, int pagina);

void insere(Lista** lista, ListaElemento* elemento);

void removeElemento(Lista** lista, ListaElemento* elemento);

void removePrimeiro(Lista** lista);

void imprimeLista(Lista* lista);

int possui(Lista* lista, ListaElemento* elemento);

void moveElementoParaOFinal(Lista** lista, ListaElemento* elemento);

#endif