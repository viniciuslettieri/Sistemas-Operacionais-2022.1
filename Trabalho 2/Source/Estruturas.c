#include <stdlib.h>
#include "Estruturas.h"

int numProcessos = 0;

Processo* CriaProcesso(int PID){
    Processo *processo = (Processo *)malloc(sizeof(Processo));

    processo->numeroPaginasMemoria = 0;
    processo->PID = numProcessos++;
    for (int i = 0; i < NUM_PAGINAS_PROCESSO; i++){
        processo->tabelaPaginas[i] = 0; // não está na memória principal
    }

    return processo;
}

Pagina* CriaPagina(int paginaID, int PID){
    Pagina *pagina = (Pagina *)malloc(sizeof(Pagina));
    
    pagina->paginaID = paginaID;
    pagina->PID = PID;

    return pagina;
}

MemoriaPrincipal* CriaMemoriaPrincipal(){
    MemoriaPrincipal *mp = (MemoriaPrincipal *)malloc(sizeof(MemoriaPrincipal));

    return mp;
}