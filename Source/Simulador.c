#include "ListaIO.h"
#include "Processo.h"

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
Fila* fila_alta;
Fila* fila_baixa;
Fila* fila_disco;
Fila* fila_fita;
Fila* fila_impressora;

// Declaração da Lista de IO
Lista* lista_io;

// Processo em Execução Corrente
Processo* processoAtivo = (Processo*) NULL;
DefinicaoProcesso* definicaoProcessoAtivo = (DefinicaoProcesso*) NULL;

int tempo_atual = 0;                    // tempo desde o inicio do escalonador
int tempo_processamento_atual = 0;      // tempo desde o inicio do quantum atual

// ================= FUNCOES ================= //

void inicializaEstruturas(){
    // seta seed aleatoria
    setaAleatorio();

    // Inicialização da Tabela de Processos
    for(int i=0; i<QUANT_PROCESSOS; i++){
        tabelaProcessos[i] = geraDefinicaoProcesso(MAX_CHEGADA, MAX_TEMPO_SERVICO);
    }

    // Inicialização das Filas de Prioridade
    fila_alta = criaFila();
    fila_baixa = criaFila();
    fila_disco = criaFila();
    fila_fita = criaFila();
    fila_impressora = criaFila();

    // Inicialização da Lista de IO
    lista_io = criaLista();
}

void trataNovosProcessos(){
    // Se Novo Processo - Cria novo PCB - AQUI PRECISA OLHAR A TABELA PARA VER SE PRECISA ADICIONAR ALGUEM
    // ...
}

void entraProximoProcesso(){
    // checa nas filas qual proximo processo vai entrar 
    // e esse processo que entrou já toma 1 instante de execucao corrente
}

void trataSaidasIO(){
    // Checar se existe algum processo saindo de IO
    // ... precisa da ListaIO
}

void trataEntradaIO(enum tipo_io tipo){
    if(tipo == IO_DISCO){
        insereVerso(fila_disco, processoAtivo);
    }else if(tipo == IO_FITA){
        insereVerso(fila_fita, processoAtivo);
    }else if(tipo == IO_IMPRESSORA){
        insereVerso(fila_impressora, processoAtivo);
    }
    processoAtivo = (Processo*) NULL;
}

void trataEntradaPreempcao(){
    // nao finalizado
    tempo_processamento_atual = 0;
    processoAtivo = (Processo*) NULL;
}

void trataProcessoAtual(){
    // Sem processos ativos
    if(processoAtivo == (Processo*) NULL){
        entraProximoProcesso();
        processoAtivo->tempoCorrente++;
        tempo_processamento_atual++;
        return;
    }

    // Trata Entrada em IO
    for(int i=0; i<MAX_IO; i++){
        if(definicaoProcessoAtivo->entradaDisco[i] == processoAtivo->tempoCorrente){
            trataEntradaIO(IO_DISCO);
            return;
        }else if(definicaoProcessoAtivo->entradaFita[i] == processoAtivo->tempoCorrente){
            trataEntradaIO(IO_FITA);
            return;
        }else if(definicaoProcessoAtivo->entradaImpressora[i] == processoAtivo->tempoCorrente){
            trataEntradaIO(IO_IMPRESSORA);
            return;
        }
    }

    if(tempo_processamento_atual == TAM_QUANTUM - 1){ 
        trataEntradaPreempcao();
    }else if(processoAtivo->tempoCorrente == definicaoProcessoAtivo->tempoDeServico - 1){ 
        trataFimProcesso();
    }else{
        processoAtivo->tempoCorrente++;
        tempo_processamento_atual++;
    }
}

void printTabelaProcessos(){
    for (int i = 0; i < QUANT_PROCESSOS; i++){
        printf("\nProcesso %d\n", i+1);
        printDefinicaoProcesso(tabelaProcessos[i]);
    }
    puts("");
}


// ================ MAIN ================ //

int main(){

    inicializaEstruturas();

    // printa tabela de processos
    printTabelaProcessos();
    /*
    // Tratamento por Unidade de Tempo    
    while( emExecucao != (Processo*) NULL ){
        trataNovosProcessos();
        trataSaidasIO();
        trataProcessoAtual();
        tempo_atual++;
    }
    */
    // FREE DA TABELA DE DEFINICAO DE PROCESSOS
    // FREE DAS FILAS
    // FREE DOS PROCESSOS

    return 0;

}