#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "Estruturas.h"
#include "Interface.h"

/* https://stackoverflow.com/questions/6127503/shuffle-array-in-c */
/* Algoritmo usado para randomizar a ordem de alocacao dos processos */
/* O Algoritmo mistura um vetor de inteiros de forma randomica */
void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void CriaOrdemAleatoriaDeProcessos(int *processos, size_t processosAtivos)
{
    for (int i = 0; i < processosAtivos; i++)
        processos[i] = i;

    shuffle(processos, processosAtivos);
}

void inserePaginaNaAreaDeSwap(Fila **areaDeSwap, FilaElemento *removido)
{
    FilaElemento *removidoSwap = Insere(areaDeSwap, removido);
    if (removidoSwap != (FilaElemento *)NULL)
    {
        printf("ÁREA DE SWAP ESTOROU!!!!\n");
        printf("CONSIDERE AUMENTAR A ÁREA SWAP!!!!\n");
    }
}

void removeDaMemoriaPrincipal(Fila **memoriaPrincipal, FilaElemento *elementoMP)
{
    if (elementoMP->anterior == (FilaElemento *)NULL)
    {
        (*memoriaPrincipal)->primeiro = elementoMP->proximo;
        elementoMP->proximo->anterior = (FilaElemento *)NULL;
    }
    else if (elementoMP->proximo == (FilaElemento *)NULL)
    {
        elementoMP->anterior->proximo = (FilaElemento *)NULL;
    }
    else
    {
        elementoMP->anterior->proximo = elementoMP->proximo;
        elementoMP->proximo->anterior = elementoMP->anterior;
    }

    (*memoriaPrincipal)->size--;
}

int main()
{
    definicoesIniciais();

    srand(time(NULL));
    Processo *filaProcessos[NUM_PROCESSOS];
    int processosAtivos = 0;

    // criar memoria principal
    Fila *memoriaPrincipal = CriaFila(NUM_FRAMES);
    Fila *areaDeSwap = CriaFila(TAM_SWAP);

    while (1)
    {
        if (processosAtivos < NUM_PROCESSOS)
        {
            filaProcessos[processosAtivos] = CriaProcesso(processosAtivos);
            processosAtivos++;
        }

        int processos[processosAtivos];
        CriaOrdemAleatoriaDeProcessos(processos, processosAtivos);

        for (int i = 0; i < processosAtivos; i++)
        {
            Pagina *pagina;
            int paginaID;
            int PID = processos[i];

            do
                paginaID = rand() % NUM_PAGINAS_PROCESSO;
            while (filaProcessos[PID]->tabelaPaginas[paginaID] != (FilaElemento *)NULL);

            // Impressao da nossa tela do simulador no MODO 1
            if( MODO == 1 ){
                printTela(memoriaPrincipal, filaProcessos, areaDeSwap, paginaID, PID, processosAtivos);
                aguardaClique();
            }

            FilaElemento *elementoSwap = BuscaElemento2(areaDeSwap, paginaID, PID);
            if (elementoSwap != (FilaElemento *)NULL)
                RemoveElemento(&areaDeSwap, elementoSwap);

            pagina = CriaPagina(paginaID, PID);

            FilaElemento *elemento = CriaElemento(memoriaPrincipal, pagina);                                // LRU da memoria principal
            FilaElemento *elemento2 = CriaElemento(filaProcessos[PID]->paginasNaMemoriaPrincipal, pagina); // LRU do processo

            if (filaProcessos[PID]->paginasNaMemoriaPrincipal->size < WORK_SET_LIMIT)
            {
                FilaElemento *removido = Insere(&memoriaPrincipal, elemento);
                AlocaPagina(pagina, memoriaPrincipal);

                if (removido != (FilaElemento *)NULL)
                {
                    int processID = removido->pagina->PID;
                    RemoveElemento(&(filaProcessos[processID]->paginasNaMemoriaPrincipal), removido);
                    filaProcessos[processID]->tabelaPaginas[removido->pagina->paginaID] = (FilaElemento *)NULL;

                    inserePaginaNaAreaDeSwap(&areaDeSwap, removido);
                }

                // Atualiza LRU do Processo que alocou a pagina
                Insere(&(filaProcessos[PID]->paginasNaMemoriaPrincipal), elemento2);

                // Atualiza tabela de paginas do Processo que alocou a pagina
                InsereElementoNaTabelaDePaginas(filaProcessos[PID], elemento);
            }
            else
            {
                //  Atualiza LRU do Processo que alocou a pagina
                FilaElemento *removido = Insere(&(filaProcessos[PID]->paginasNaMemoriaPrincipal), elemento2);

                // pega ponteiro da tabela de paginas da pagina a ser removida
                FilaElemento *elementoMP = filaProcessos[PID]->tabelaPaginas[removido->pagina->paginaID];
                removeDaMemoriaPrincipal(&memoriaPrincipal, elementoMP);
                // remove da tabela de paginas
                filaProcessos[PID]->tabelaPaginas[removido->pagina->paginaID] = (FilaElemento *)NULL;

                Insere(&memoriaPrincipal, elemento);
                AlocaPagina(pagina, memoriaPrincipal);

                inserePaginaNaAreaDeSwap(&areaDeSwap, removido);

                // Atualiza tabela de paginas do Processo que alocou a pagina
                InsereElementoNaTabelaDePaginas(filaProcessos[PID], elemento);
            }

            // sleep(1);
        }

        // Impressao da tela no MODO 2
        if( MODO == 2 ){
            printTela(memoriaPrincipal, filaProcessos, areaDeSwap, -1, -1, processosAtivos);
            sleep(INTERVALO);
        }
    }

    return 0;
}