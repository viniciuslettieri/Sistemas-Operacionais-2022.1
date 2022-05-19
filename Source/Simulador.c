#include "Process.h"
#include "Queue.h"
#include "Util.h"

int main(){

    // Criacao da Tabela
    DefinicaoProcesso* tabelaProcessos[QUANT_PROCESSOS];
    for(int i=0; i<QUANT_PROCESSOS; i++){
        tabelaProcessos[i] = geraDefinicaoProcesso(MAX_CHEGADA, MAX_TEMPO_SERVICO);
    }

    // Filas de Prioridade
    Queue* fila_alta = createQueue();
    Queue* fila_baixa = createQueue();
    Queue* fila_disco = createQueue();
    Queue* fila_fita = createQueue();
    Queue* fila_impressora = createQueue();

    // Lista de IO
    ListaIO* lista_io = createList();
    [tempo entrada, tempo de saida, tipo de io, ponteiro para o Processo*]

    // Tratamento por Unidade de Tempo
    int tempo_atual = 0;
    Processo* emExecucao = (Processo*) NULL;
    while( processosAtivos() > 0 ){
        // Se Novo Processo - Cria novo PCB - AQUI PRECISA OLHAR A TABELA PARA VER SE PRECISA ADICIONAR ALGUEM
        Processo* novoProcesso = (Processo*) malloc( sizeof(Processo) );
        push_back(fila_alta, novoProcesso);

        // Checar se existe algum processo saindo de IO
        // ... precisa da ListaIO

        // Realiza Preempção do processo em execução
    }

    // FREE DA TABELA DE DEFINICAO DE PROCESSOS
    // FREE DAS FILAS
    // FREE DOS PROCESSOS

    return 0;

}