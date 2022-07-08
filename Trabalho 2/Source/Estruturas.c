#include <stdlib.h>
#include <stdio.h>
#include "Estruturas.h"

int numProcessos = 0;

Processo* CriaProcesso(int PID){
    Processo *processo = (Processo *)malloc(sizeof(Processo));

    processo->PID = numProcessos++;
    for (int i = 0; i < NUM_PAGINAS_PROCESSO; i++){
        processo->tabelaPaginas[i] = (ListaElemento*) NULL; // não está na memória principal
    }
    processo->paginasNaMemoriaPrincipal = CriaLista(WORK_SET_LIMIT);
    return processo;
}

Pagina* CriaPagina(int paginaID, int PID){
    Pagina *pagina = (Pagina *)malloc(sizeof(Pagina));
    
    pagina->paginaID = paginaID;
    pagina->PID = PID;

    return pagina;
}

void InsereElementoNaTabelaDePaginas(Processo* processo, ListaElemento* elemento){
    processo->tabelaPaginas[elemento->pagina->paginaID] = elemento;
}

void ImprimeTabelaDePaginas(Processo* processo){
    printf("Tabela de Paginas do Processo [%d]: \n", processo->PID);
    for (int i = 0; i < NUM_PAGINAS_PROCESSO; i++){
        if (processo->tabelaPaginas[i] != NULL){
            printf("%d ", processo->tabelaPaginas[i]->pagina->paginaID);
        }
    }
    printf("\n");
}
