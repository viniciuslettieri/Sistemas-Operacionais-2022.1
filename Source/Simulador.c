#include "Process.h"
#include "Queue.h"
#include "ListaIO.h"
#include "Util.h"


/* 
-----------------------------
Listagem de Pensamento::

Inicio -> Filas Vazias -> Lista de IO Vazia

A cada unidade de tempo verificamos se existe um novo processo sendo criado.
Se sim, precisa entrar na fila de alta prioridade.

Quando um processo inicia o IO, ele é removido de sua fila e adicionado na Lista de IO, com seu tempo de finalização

A cada unidade de tempo precisamos verificar se um processo finalizou seu IO, e caso positivo, será adicionado na sua respectiva fila.

-----------------------------
*/


// =============== DECLARACOES =============== //

// Declaração da Tabela de Processos
DefinicaoProcesso* tabelaProcessos[QUANT_PROCESSOS];

// Declaração das Filas de Prioridade
Queue* fila_alta;
Queue* fila_baixa;
Queue* fila_disco;
Queue* fila_fita;
Queue* fila_impressora;

// Declaração da Lista de IO
ListaIO* lista_io;

// Processo em Execução Corrente
Processo* emExecucao = (Processo*) NULL;
int tempo_atual = 0;


// ================= FUNCOES ================= //

void inicializaEstruturas(){
    // Inicialização da Tabela de Processos
    for(int i=0; i<QUANT_PROCESSOS; i++){
        tabelaProcessos[i] = geraDefinicaoProcesso(MAX_CHEGADA, MAX_TEMPO_SERVICO);
    }

    // Inicialização das Filas de Prioridade
    fila_alta = createQueue();
    fila_baixa = createQueue();
    fila_disco = createQueue();
    fila_fita = createQueue();
    fila_impressora = createQueue();

    // Inicialização da Lista de IO
    lista_io = createList();
}

void trataNovosProcessos(){
    // Se Novo Processo - Cria novo PCB - AQUI PRECISA OLHAR A TABELA PARA VER SE PRECISA ADICIONAR ALGUEM
    // ...
}

void trataSaidasIO(){
    // Checar se existe algum processo saindo de IO
    // ... precisa da ListaIO
}

void entraIO(){

}

void entraPreempcao(){

}

void trataProcessoAtual(){
    // verifica se entraIO();
    // ou verifica se entraPreempcao();
    // ou verifica se terminou (precisa dar free)
    // ou entao nada
}


// ================ MAIN ================ //

int main(){

    inicializaEstruturas();

    // Tratamento por Unidade de Tempo    
    while( emExecucao != (Processo*) NULL ){
        trataNovosProcessos();
        trataSaidasIO();
        trataProcessoAtual();
        tempo_atual++;
    }

    // FREE DA TABELA DE DEFINICAO DE PROCESSOS
    // FREE DAS FILAS
    // FREE DOS PROCESSOS

    return 0;

}