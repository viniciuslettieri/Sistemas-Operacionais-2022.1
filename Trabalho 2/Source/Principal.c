#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "Estruturas.h"

/*

* Definir uma estrutura TabelaPaginas
    Cada entrada possui um marcador se está na memória

* Definir uma estrutura Processo
    Processo possui uma tabela de paginas

*/

/* https://stackoverflow.com/questions/6127503/shuffle-array-in-c */
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

int main()
{

    srand(time(NULL));
    Processo *listaProcessos[NUM_PROCESSOS];
    int processosAtivos = 0;

    // criar memoria principal
    Lista *memoriaPrincipal = CriaLista(NUM_FRAMES);

    while (1)
    {
        if (processosAtivos < NUM_PROCESSOS)
        {
            // CriaProcesso
            listaProcessos[processosAtivos] = CriaProcesso(processosAtivos);
            processosAtivos++;
        }

        int processos[processosAtivos];
        for (int i = 0; i < processosAtivos; i++)
        {
            processos[i] = i;
        }

        shuffle(processos, processosAtivos);

        for (int i = 0; i < processosAtivos; i++)
        {
            Pagina *pagina;
            int paginaID;
            int PID = processos[i];

            do
            {
                paginaID = rand() % NUM_PAGINAS_PROCESSO;
            } while (listaProcessos[PID]->tabelaPaginas[paginaID] != (ListaElemento *)NULL);

            pagina = CriaPagina(paginaID, PID);
            ListaElemento *elemento = CriaElemento(memoriaPrincipal, pagina);
            ListaElemento *elemento2 = CriaElemento(listaProcessos[PID]->paginasNaMemoriaPrincipal, pagina);

            if (listaProcessos[PID]->paginasNaMemoriaPrincipal->size < WORK_SET_LIMIT)
            {
                // printf("Abaixo do WORK_SET_lIMIT!!\n");
                ListaElemento *removido = Insere(&memoriaPrincipal, elemento);
                if (removido != NULL)
                {
                    int processID = removido->pagina->PID;
                    RemoveElemento(&(listaProcessos[processID]->paginasNaMemoriaPrincipal), removido);
                    listaProcessos[processID]->tabelaPaginas[removido->pagina->paginaID] = (ListaElemento *)NULL;
                    free(removido);

                    /*
                    TODO (Thierry):

                    Implementar condicao, estrutura e área de Swap
                    */
                }

                // Atualiza LRU do Processo que alocou a pagina
                Insere(&(listaProcessos[PID]->paginasNaMemoriaPrincipal), elemento2);
                printf("LRU Processo [%d]: \n", listaProcessos[PID]->PID);
                ImprimeLista(listaProcessos[PID]->paginasNaMemoriaPrincipal);

                // Atualiza tabela de paginas do Processo que alocou a pagina
                InsereElementoNaTabelaDePaginas(listaProcessos[PID], elemento);
                // ImprimeTabelaDePaginas(listaProcessos[PID]);
            }
            else
            {
                // printf("Atingiu WORK_SET_lIMIT!!\n");
                //  Atualiza LRU do Processo que alocou a pagina
                ListaElemento *removido = Insere(&(listaProcessos[PID]->paginasNaMemoriaPrincipal), elemento2);
                // printf("O \n");
                // printf("Removido: (%d,%d)\n", removido->pagina->paginaID, removido->pagina->PID);
                //  sabemos que ocorreu uma remoção do LRU do Processo. Então, precisamos atualizar a memoria
                //  principal de uma forma mais especifica.

                // pega ponteiro da tabela de paginas da pagina a ser removida
                ListaElemento *elementoMP = listaProcessos[PID]->tabelaPaginas[removido->pagina->paginaID];

                // remove da tabela de paginas
                listaProcessos[PID]->tabelaPaginas[removido->pagina->paginaID] = (ListaElemento *)NULL;

                // printf("Elemento: (%d,%d)\n", elementoMP->pagina->paginaID, elementoMP->pagina->PID);
                // printf("\niniciou\n");
                if (elementoMP->anterior == (ListaElemento *)NULL)
                {
                    // printf("\n1\n");
                    //  caso anterior é nulo att ponteiro mp - mais antigo
                    memoriaPrincipal->primeiro = elementoMP->proximo;
                    elementoMP->proximo->anterior = (ListaElemento *)NULL;
                }
                else if (elementoMP->proximo == (ListaElemento *)NULL)
                {
                    // printf("\n2\n");
                    //  caso prox é nulo
                    elementoMP->anterior->proximo = (ListaElemento *)NULL;
                }
                else
                {
                    // printf("\n3\n");
                    //  caso normal
                    elementoMP->anterior->proximo = elementoMP->proximo;
                    elementoMP->proximo->anterior = elementoMP->anterior;
                }

                memoriaPrincipal->size--;

                Insere(&memoriaPrincipal, elemento);

                printf("LRU Processo [%d]: \n", listaProcessos[PID]->PID);
                ImprimeLista(listaProcessos[PID]->paginasNaMemoriaPrincipal);

                // Atualiza tabela de paginas do Processo que alocou a pagina
                InsereElementoNaTabelaDePaginas(listaProcessos[PID], elemento);
                // ImprimeTabelaDePaginas(listaProcessos[PID]);
            }
        }

        printf("Memoria Principal: \n");
        ImprimeLista(memoriaPrincipal);

        sleep(INTERVALO);
    }

    /*
        TODO (Luan): Tornar a Lista de LRU duplamente encadeada
    */

    return 0;
}