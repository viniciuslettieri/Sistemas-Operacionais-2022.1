#ifndef LISTA_H
#define LISTA_H

// Valores bons de testagem:
// #define NUM_PAGINAS_PROCESSO 10
// #define NUM_FRAMES 20
// #define NUM_PROCESSOS 5

#define NUM_PAGINAS_PROCESSO 50
#define NUM_FRAMES 64
#define INTERVALO 3                 // paginas e processos sendo criados a cada INTERVALO
#define NUM_PROCESSOS 20
#define WORK_SET_LIMIT 4
#define TAM_SWAP 500

// Definicao do MODO de apresentacao
// MODO 1 aguarda clique para cada insercao
// MODO 2 gera insercoes para todos os processos a cada 3 segundos
#define MODO 2

typedef struct _Pagina
{
    int paginaID;
    int PID;
    int frameIndex;                 // usado para identificar o frame de alocacao (ignorar nos outros locais)
} Pagina;

typedef struct _ListaElemento
{
    struct _ListaElemento *anterior;
    Pagina *pagina;
    struct _ListaElemento *proximo;
} ListaElemento;

typedef struct _Lista
{
    int size;
    int tamanhoMaximo;
    ListaElemento *primeiro;
} Lista;

typedef struct _Processo
{
    int PID;
    ListaElemento *tabelaPaginas[NUM_PAGINAS_PROCESSO];
    Lista *paginasNaMemoriaPrincipal;
} Processo;

Processo *CriaProcesso(int PID);
Pagina *CriaPagina(int paginaID, int PID);
void AlocaPagina(Pagina* pagina, Lista* memoriaPrincipal);
Lista *CriaLista(int tamanhoMaximo);
ListaElemento *BuscaElemento(Lista *lista, Pagina *pagina);
ListaElemento *BuscaElemento2(Lista *lista, int paginaID, int PID);
ListaElemento *CriaElemento(Lista *, Pagina *pagina);
ListaElemento *Insere(Lista **lista, ListaElemento *elemento);
void RemoveElemento(Lista **lista, ListaElemento *elemento);
ListaElemento *RemovePrimeiro(Lista **lista);
void ImprimeLista(Lista *lista);
int Possui(Lista *lista, ListaElemento *elemento);
int Igual(Pagina *a, Pagina *b);
int Igual2(Pagina *a, int paginaID, int PID);
void MoveElementoParaOFinal(Lista **lista, ListaElemento *elemento);
void InsereElementoNaTabelaDePaginas(Processo *processo, ListaElemento *elemento);
void ImprimeTabelaDePaginas(Processo *processo);
#endif