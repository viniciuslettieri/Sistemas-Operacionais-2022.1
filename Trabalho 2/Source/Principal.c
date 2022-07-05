#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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
    while(1){
        if (processosAtivos < NUM_PROCESSOS){
            //CriaProcesso
            listaProcessos[processosAtivos] = CriaProcesso(processosAtivos);
            processosAtivos++;
        }

        for(int i = 0; i < processosAtivos; i++){
            Pagina* pagina = CriaPagina(rand() % NUM_PAGINAS_PROCESSO,i);
            // deveria alocar na memoria principal a pagina criada
        }

        sleep(INTERVALO);
        
    }
    return 0;
}