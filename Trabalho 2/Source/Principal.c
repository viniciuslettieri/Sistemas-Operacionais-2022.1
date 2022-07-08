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

int main() {

    srand(time(NULL));
    Processo* listaProcessos[NUM_PROCESSOS];
    int processosAtivos = 0;

    // criar memoria principal
    Lista* memoriaPrincipal = CriaLista(NUM_FRAMES);

    while(1){
        if (processosAtivos < NUM_PROCESSOS){
            //CriaProcesso
            listaProcessos[processosAtivos] = CriaProcesso(processosAtivos);
            processosAtivos++;
        }

        for(int i = 0; i < processosAtivos; i++){
            Pagina* pagina = CriaPagina(rand() % NUM_PAGINAS_PROCESSO,i);

            ListaElemento* elemento = CriaElemento(memoriaPrincipal, pagina);
            ListaElemento* elemento2 = CriaElemento(listaProcessos[i]->paginasNaMemoriaPrincipal, pagina);

            if (listaProcessos[i]->paginasNaMemoriaPrincipal->size < WORK_SET_LIMIT){

                ListaElemento* removido = Insere(&memoriaPrincipal, elemento);
                if (removido != NULL){
                    int processID = removido->pagina->PID;
                    RemoveElemento(&(listaProcessos[processID]->paginasNaMemoriaPrincipal), removido);
                    listaProcessos[processID]->tabelaPaginas[removido->pagina->paginaID] = (ListaElemento*) NULL;
                    free(removido);

                    /*
                    TODO (Thierry):

                    Implementar condicao, estrutura e área de Swap
                    */
                }

                //Atualiza LRU do Processo que alocou a pagina
                Insere(&(listaProcessos[i]->paginasNaMemoriaPrincipal), elemento2);
                printf("LRU Processo [%d]: \n", listaProcessos[i]->PID);
                ImprimeLista(listaProcessos[i]->paginasNaMemoriaPrincipal);

                //Atualiza tabela de paginas do Processo que alocou a pagina 
                InsereElementoNaTabelaDePaginas(listaProcessos[i], elemento);
                //ImprimeTabelaDePaginas(listaProcessos[i]);

            } else {

                //Atualiza LRU do Processo que alocou a pagina
                ListaElemento* removido2 = Insere(&(listaProcessos[i]->paginasNaMemoriaPrincipal), elemento2);

                // sabemos que ocorreu uma remoção do LRU do Processo. Então, precisamos atualizar a memoria
                // principal de uma forma mais especifica.
                
                /*
                TODO (Thierry):

                Com a lista duplamente encadeada, remover pagina do LRU do processo obtendo-a como retorno
                para alterar a tabela de paginas e obter acesso ao ponteiro da memoria principal. Assim,
                bastará remover da memoria principal e inserir a nova pagina.

                // REMOVE DO LRU DO PROCESSO - ACESSA TABELA DE PAGINAS PARA PEGAR O PONTEIRO PARA A MEMORIA PRINCIPAL
                // - REMOVE DIRETAMENTE NA MEMORIA PRINCIPAL
                */
                
                printf("LRU Processo [%d]: \n", listaProcessos[i]->PID);
                ImprimeLista(listaProcessos[i]->paginasNaMemoriaPrincipal);

                //Atualiza tabela de paginas do Processo que alocou a pagina 
                InsereElementoNaTabelaDePaginas(listaProcessos[i], elemento);
                //ImprimeTabelaDePaginas(listaProcessos[i]);
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