#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Estruturas.h"

int numProcessos = 0;

Processo *CriaProcesso(int PID)
{
    Processo *processo = (Processo *)malloc(sizeof(Processo));

    processo->PID = numProcessos++;
    for (int i = 0; i < NUM_PAGINAS_PROCESSO; i++)
    {
        processo->tabelaPaginas[i] = (ListaElemento *)NULL; // não está na memória principal
    }
    processo->paginasNaMemoriaPrincipal = CriaLista(WORK_SET_LIMIT);
    return processo;
}

Pagina *CriaPagina(int paginaID, int PID)
{
    Pagina *pagina = (Pagina *)malloc(sizeof(Pagina));

    pagina->paginaID = paginaID;
    pagina->PID = PID;
    pagina->frameIndex = -1;

    return pagina;
}

void AlocaPagina(Pagina* pagina, Lista* memoriaPrincipal){
    int alocacoes[NUM_FRAMES];
    memset(alocacoes, 0, sizeof(alocacoes));

    // Define a lista dos frames usados
    ListaElemento *p = memoriaPrincipal->primeiro;
    while (p != NULL)
    {
        if (p->pagina->frameIndex != -1)
            alocacoes[p->pagina->frameIndex] = 1;
        p = p->proximo;
    }

    // Encontra um frame vazio
    for(int i=0; i<NUM_FRAMES; i++){
        if(alocacoes[i] == 0){
            pagina->frameIndex = i;
            return;
        }
    }
}

void InsereElementoNaTabelaDePaginas(Processo* processo, ListaElemento* elemento){
    processo->tabelaPaginas[elemento->pagina->paginaID] = elemento;
}

void ImprimeTabelaDePaginas(Processo *processo)
{
    printf("Tabela de Paginas do Processo [%d]: \n", processo->PID);
    for (int i = 0; i < NUM_PAGINAS_PROCESSO; i++)
    {
        if (processo->tabelaPaginas[i] != NULL)
        {
            printf("%d ", processo->tabelaPaginas[i]->pagina->paginaID);
        }
    }
    printf("\n");
}

// Verifica se duas páginas são iguais
int Igual(Pagina *a, Pagina *b)
{
    return a->paginaID == b->paginaID && a->PID == b->PID;
}

// Verifica se duas páginas são iguais
int Igual2(Pagina *a, int paginaID, int PID)
{
    return a->paginaID == paginaID && a->PID == PID;
}
