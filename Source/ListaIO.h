#include "Util.h"
#include "Processo.h"

typedef struct _ListaElemento {
    int tempo_entrada, tempo_saida;
    int tipo_io;
    Processo* processo;
    struct _ListaElemento* next;
} ListaElemento;
 
typedef struct _Lista {
    int size;
    ListaElemento *prim;
} ListaIO;

// IMPLEMENTAR OPERACOES DE LISTA ENCADEADA