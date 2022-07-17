#ifndef FILA_H
#define FILA_H

// Valores bons de testagem:
// #define NUM_PAGINAS_PROCESSO 10
// #define NUM_FRAMES 20
// #define NUM_PROCESSOS 5

#define NUM_PAGINAS_PROCESSO 50
#define NUM_FRAMES 64
#define NUM_PROCESSOS 20
#define INTERVALO 3                 // paginas e processos sendo criados a cada INTERVALO
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

typedef struct _FilaElemento
{
    struct _FilaElemento *anterior;
    Pagina *pagina;
    struct _FilaElemento *proximo;
} FilaElemento;

typedef struct _Fila
{
    int size;
    int tamanhoMaximo;
    FilaElemento *primeiro;
} Fila;

typedef struct _Processo
{
    int PID;
    FilaElemento *tabelaPaginas[NUM_PAGINAS_PROCESSO];
    Fila *paginasNaMemoriaPrincipal;
} Processo;

Processo *CriaProcesso(int PID);
Pagina *CriaPagina(int paginaID, int PID);
void AlocaPagina(Pagina* pagina, Fila* memoriaPrincipal);
Fila *CriaFila(int tamanhoMaximo);
FilaElemento *BuscaElemento(Fila *fila, Pagina *pagina);
FilaElemento *BuscaElemento2(Fila *fila, int paginaID, int PID);
FilaElemento *CriaElemento(Fila *, Pagina *pagina);
FilaElemento *Insere(Fila **fila, FilaElemento *elemento);
void RemoveElemento(Fila **fila, FilaElemento *elemento);
FilaElemento *RemovePrimeiro(Fila **fila);
void ImprimeFila(Fila *fila);
int Possui(Fila *fila, FilaElemento *elemento);
int Igual(Pagina *a, Pagina *b);
int Igual2(Pagina *a, int paginaID, int PID);
void MoveElementoParaOFinal(Fila **fila, FilaElemento *elemento);
void InsereElementoNaTabelaDePaginas(Processo *processo, FilaElemento *elemento);
void ImprimeTabelaDePaginas(Processo *processo);
#endif