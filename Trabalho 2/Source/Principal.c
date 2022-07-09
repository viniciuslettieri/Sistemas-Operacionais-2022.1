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

void inserePaginaNaAreaDeSwap(Lista **areaDeSwap, ListaElemento *removido)
{
    ListaElemento *removidoSwap = Insere(areaDeSwap, removido);
    if (removidoSwap != (ListaElemento *)NULL)
    {
        printf("ÁREA DE SWAP ESTOROU!!!!\n");
        printf("CONSIDERE AUMENTAR A ÁREA SWAP!!!!\n");
    }
}

void removeDaMemoriaPrincipal(Lista **memoriaPrincipal, ListaElemento *elementoMP)
{
    if (elementoMP->anterior == (ListaElemento *)NULL)
    {
        (*memoriaPrincipal)->primeiro = elementoMP->proximo;
        elementoMP->proximo->anterior = (ListaElemento *)NULL;
    }
    else if (elementoMP->proximo == (ListaElemento *)NULL)
    {
        elementoMP->anterior->proximo = (ListaElemento *)NULL;
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
    Processo *listaProcessos[NUM_PROCESSOS];
    int processosAtivos = 0;

    // criar memoria principal
    Lista *memoriaPrincipal = CriaLista(NUM_FRAMES);
    Lista *areaDeSwap = CriaLista(TAM_SWAP);

    while (1)
    {
        if (processosAtivos < NUM_PROCESSOS)
        {
            listaProcessos[processosAtivos] = CriaProcesso(processosAtivos);
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
            while (listaProcessos[PID]->tabelaPaginas[paginaID] != (ListaElemento *)NULL);

            // Impressao da nossa tela do simulador
            printTela(memoriaPrincipal, listaProcessos, areaDeSwap, paginaID, PID, processosAtivos);
            // aguardaClique();

            ListaElemento *elementoSwap = BuscaElemento2(areaDeSwap, paginaID, PID);
            if (elementoSwap != (ListaElemento *)NULL)
                RemoveElemento(&areaDeSwap, elementoSwap);

            pagina = CriaPagina(paginaID, PID);

            ListaElemento *elemento = CriaElemento(memoriaPrincipal, pagina);                                // LRU da memoria principal
            ListaElemento *elemento2 = CriaElemento(listaProcessos[PID]->paginasNaMemoriaPrincipal, pagina); // LRU do processo

            if (listaProcessos[PID]->paginasNaMemoriaPrincipal->size < WORK_SET_LIMIT)
            {
                ListaElemento *removido = Insere(&memoriaPrincipal, elemento);
                AlocaPagina(pagina, memoriaPrincipal);

                if (removido != (ListaElemento *)NULL)
                {
                    int processID = removido->pagina->PID;
                    RemoveElemento(&(listaProcessos[processID]->paginasNaMemoriaPrincipal), removido);
                    listaProcessos[processID]->tabelaPaginas[removido->pagina->paginaID] = (ListaElemento *)NULL;

                    inserePaginaNaAreaDeSwap(&areaDeSwap, removido);
                }

                // Atualiza LRU do Processo que alocou a pagina
                Insere(&(listaProcessos[PID]->paginasNaMemoriaPrincipal), elemento2);

                // Atualiza tabela de paginas do Processo que alocou a pagina
                InsereElementoNaTabelaDePaginas(listaProcessos[PID], elemento);
            }
            else
            {
                //  Atualiza LRU do Processo que alocou a pagina
                ListaElemento *removido = Insere(&(listaProcessos[PID]->paginasNaMemoriaPrincipal), elemento2);

                // pega ponteiro da tabela de paginas da pagina a ser removida
                ListaElemento *elementoMP = listaProcessos[PID]->tabelaPaginas[removido->pagina->paginaID];
                removeDaMemoriaPrincipal(&memoriaPrincipal, elementoMP);
                // remove da tabela de paginas
                listaProcessos[PID]->tabelaPaginas[removido->pagina->paginaID] = (ListaElemento *)NULL;

                Insere(&memoriaPrincipal, elemento);
                AlocaPagina(pagina, memoriaPrincipal);

                inserePaginaNaAreaDeSwap(&areaDeSwap, removido);

                // Atualiza tabela de paginas do Processo que alocou a pagina
                InsereElementoNaTabelaDePaginas(listaProcessos[PID], elemento);
            }
        }

        sleep(INTERVALO);
    }

    return 0;
}