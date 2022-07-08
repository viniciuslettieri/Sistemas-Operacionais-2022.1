#ifndef LISTA_H
#define LISTA_H

#define NUM_PAGINAS_PROCESSO 50
#define NUM_FRAMES 64
#define INTERVALO 3 // paginas e processos sendo criados a cada INTERVALO
#define NUM_PROCESSOS 20
#define WORK_SET_LIMIT 4

typedef struct _Pagina {
    int paginaID;
    int PID;
} Pagina;

typedef struct _ListaElemento {
    Pagina* pagina;
    struct _ListaElemento* proximo;
} ListaElemento;

typedef struct _Lista {
    int size;
    int tamanhoMaximo;
    ListaElemento* primeiro;
} Lista;

typedef struct _Processo {
    int PID;
    ListaElemento* tabelaPaginas[NUM_PAGINAS_PROCESSO];
    Lista* paginasNaMemoriaPrincipal; 
} Processo;

Processo* CriaProcesso(int PID);
Pagina* CriaPagina(int paginaID, int PID);
Lista* CriaLista(int tamanhoMaximo);
ListaElemento* BuscaElemento(Lista* lista, Pagina* pagina);
ListaElemento* CriaElemento(Lista*, Pagina* pagina);
ListaElemento* Insere(Lista** lista, ListaElemento* elemento);
void RemoveElemento(Lista** lista, ListaElemento* elemento);
ListaElemento* RemovePrimeiro(Lista** lista);
void ImprimeLista(Lista* lista);
int Possui(Lista* lista, ListaElemento* elemento);
void MoveElementoParaOFinal(Lista** lista, ListaElemento* elemento);
void InsereElementoNaTabelaDePaginas(Processo* processo, ListaElemento* elemento);
void ImprimeTabelaDePaginas(Processo* processo);
#endif