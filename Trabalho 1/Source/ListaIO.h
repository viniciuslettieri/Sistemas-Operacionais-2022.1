#ifndef LISTAIO_H
#define LISTAIO_H

#include "Util.h"
#include "Processo.h"

typedef struct _ListaElemento {
    int tempo_entrada, tempo_saida;
    int tipo_io;
    Processo* processo;
    struct _ListaElemento* proximo;
} ListaElemento;

typedef struct _Lista {
    int size;
    ListaElemento* primeiro;
} Lista;

Lista* criaLista();

void excluiListaIO(Lista* lista);

ListaElemento* criaElemento(int tmp_ent, int tmp_sai, int t_io, Processo* p);

void insere(Lista* lista, ListaElemento* elemento);

ListaElemento* removeElemento(Lista* lista, ListaElemento* elemento);

void imprimeLista(Lista* lista);

ListaElemento* pegaPrimeiroDaLista(Lista* lista);

#endif