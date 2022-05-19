#include "Util.h"
#include "Process.h"

typedef struct _ListaElemento {
    int tempo_entrada, tempo_saida;
    int tipo_io;
    Processo* processo;
    struct _ListaElemento* next;
} ListaElemento;
 
typedef struct _Lista {
    int size;
    ListaElemento *prim;
} Lista;

// IMPLEMENTAR OPERACOES DE LISTA ENCADEADA