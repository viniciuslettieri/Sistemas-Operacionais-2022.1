#include "Process.h"

typedef struct _ListaElemento {
    int tempo_entrada, tempo_saida;
    int tipo_io;
    Processo* processo;
    struct _ListaElemento* proximo;
} ListaElemento;
 
typedef struct _Lista {
    int size;
    ListaElemento *primeiro;
} Lista;

// IMPLEMENTAR OPERACOES DE LISTA ENCADEADA

Lista* criaListaIO(){
    Lista* lista = (Lista*)malloc(sizeof(Lista));
    lista->primeiro = NULL;
    lista->size = 0;
    return lista;
}

void excluiListaIO(Lista* lista){
    free(lista);
}

ListaElemento* criaElemento(int tmp_ent, int tmp_sai, int t_io, Processo* p){
    ListaElemento* elemento = (ListaElemento*)malloc(sizeof(ListaElemento));
    elemento->tempo_entrada = tmp_ent;
    elemento->tempo_saida = tmp_sai;
    elemento->tipo_io = t_io;
    elemento->processo = p;
    elemento->proximo = NULL;
    return elemento;
}

void insere(Lista* lista, ListaElemento* elemento){
    if (lista->primeiro == NULL){
        lista->primeiro = elemento;
    } else {
        ListaElemento* p = lista->primeiro;
        while(p->proximo != NULL) {
            p = p->proximo;
        }
        p->proximo = elemento;
    }
    lista->size++;
}

void removeElemento(Lista* lista){
    if (lista->primeiro == NULL) return;

    ListaElemento* primeiroDaLista = lista->primeiro;
    if (primeiroDaLista->proximo == NULL){
        lista->primeiro = NULL;
    } else {
        lista->primeiro = primeiroDaLista->proximo;
    }

    lista->size--;
    free(primeiroDaLista);
}

void imprimeLista(Lista* lista){
    ListaElemento* p = lista->primeiro;
    while (p != NULL){
        printf("%d, %d, %d\n", p->tempo_entrada, p->tempo_saida, p->tipo_io);
        p = p->proximo;
    }
    printf("%d\n", lista->size);
}

ListaElemento* pegaPrimeiroDaLista(Lista* lista){
    return lista->primeiro;
}