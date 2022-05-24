#include <stdio.h>
#include <stdlib.h>

#include "ListaIO.h"



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


// Informacoes Globais
int tempo_atual = 0;                    // tempo desde o inicio do escalonador
int tempo_processamento_atual = 0;      // tempo desde o inicio do quantum atual
int processos_criados = 0;              // quantidade de processos criados ate o momento
int processos_finalizados = 0;          // quantidade de processos finalizados ate o momento



// ================= FUNCOES ================= //

void inicializaEstruturas(){
    // Define seed para gerar valores aleatorios
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
    // Novos Processos no Instante Atual sao Inseridos na Fila de Alta Prioridade
    printf("Tempos de chegada: ");
    for (int i = 0; i < QUANT_PROCESSOS; i++){
        printf("%d ", tabelaProcessos[i]->tempoDeChegada);
        if(tabelaProcessos[i]->tempoDeChegada == tempo_atual){
            Processo* aux = (Processo*) malloc(sizeof(Processo));
            aux->PID = ++processos_criados;
            aux->PPID = 0;
            aux->status = PRONTO;
            aux->tempoCorrente = 0;

            tabelaProcessos[i]->PID_relacionado = aux->PID;

            insereVerso(fila_alta, aux);

            printf("\n[++] Novo Processo %d Criado!\n", processos_criados);
        }
    }
    puts("\n");
}

void entraProximoProcesso(){
    // Encontra o Proximo Processo Pronto para Execucao
    if (fila_alta->tam > 0) {
        processoAtivo = removeFrente(fila_alta);
        processoAtivo->tempoCorrente++;
        tempo_processamento_atual = 0;
    } else if (fila_baixa->tam > 0) {
        processoAtivo = removeFrente(fila_baixa);
        processoAtivo->tempoCorrente++;
        tempo_processamento_atual = 0;
    } else {
        printf("\nSem processos prontos!\n");
        return;
    }

    printf("\n[+] Processo %d Iniciou Execucao!\n", processoAtivo->PID);

    for(int i = 0; i < QUANT_PROCESSOS; i++){
        if(tabelaProcessos[i]->PID_relacionado != -1 && tabelaProcessos[i]->PID_relacionado == processoAtivo->PID){
            definicaoProcessoAtivo = tabelaProcessos[i];
            return;
        }
    }
}

void trataSaidasIO(){
    ListaElemento* atual = lista_io->primeiro;
    while(atual != NULL){
        if (atual->tempo_saida == tempo_atual){

            removeElemento(lista_io, atual);

            if (atual->tipo_io == DISCO){
                removeFrente(fila_disco);
                insereVerso(fila_baixa, atual->processo);
            } else if (atual->tipo_io == FITA){
                removeFrente(fila_fita);
                insereVerso(fila_alta, atual->processo);
            } else {
                removeFrente(fila_impressora);
                insereVerso(fila_alta, atual->processo);
            } 

            printf("\n[-] Processo %d Saiu de IO!\n", atual->processo->PID);
            atual = atual->proximo;
            
            //free(elemento);

            
        } else {
            atual = atual->proximo;
        }       
    }
}

void trataEntradaIO(enum tipo_io tipo){
    printf("\n[-] Processo %d Entrou em IO!\n", processoAtivo->PID);

    if(tipo == IO_DISCO){
        insereVerso(fila_disco, processoAtivo);
        insere(lista_io, criaElemento(tempo_atual, tempo_atual + TEMPO_DISCO, DISCO, processoAtivo));
    }else if(tipo == IO_FITA){
        insereVerso(fila_fita, processoAtivo);
        insere(lista_io, criaElemento(tempo_atual, tempo_atual + TEMPO_FITA, FITA, processoAtivo));
    }else if(tipo == IO_IMPRESSORA){
        insereVerso(fila_impressora, processoAtivo);
        insere(lista_io, criaElemento(tempo_atual, tempo_atual + TEMPO_IMPRESSORA, IMPRESSORA, processoAtivo));
    }
    
    processoAtivo = (Processo*) NULL;
    definicaoProcessoAtivo = (DefinicaoProcesso*) NULL;
}

void trataEntradaPreempcao(){
    printf("\n[-] Processo %d Entrou em Preempcao!\n", processoAtivo->PID);

    // nao finalizado
    tempo_processamento_atual = 0;
    insereVerso(fila_baixa, processoAtivo);
    processoAtivo = (Processo*) NULL;
    definicaoProcessoAtivo = (DefinicaoProcesso*) NULL;
}

void trataFimProcesso(){
    printf("\n[--] Processo %d Finalizado!\n", processoAtivo->PID);

    free(processoAtivo);
    free(definicaoProcessoAtivo);
    processoAtivo = (Processo*) NULL;
    definicaoProcessoAtivo = (DefinicaoProcesso*) NULL;
    processos_finalizados++;
}

void trataProcessoAtual(){
    //Trata Entrada em IO

    if (processoAtivo != NULL){
        for(int i=0; i<MAX_IO; i++){
            if (processoAtivo == NULL) break;

            if(definicaoProcessoAtivo->entradaDisco[i] == processoAtivo->tempoCorrente){
                trataEntradaIO(IO_DISCO);
            }else if(definicaoProcessoAtivo->entradaFita[i] == processoAtivo->tempoCorrente){
                trataEntradaIO(IO_FITA);
            }else if(definicaoProcessoAtivo->entradaImpressora[i] == processoAtivo->tempoCorrente){
                trataEntradaIO(IO_IMPRESSORA);
            }
        }
    }
    
    if(processoAtivo != (Processo*) NULL){
        if(processoAtivo->tempoCorrente == definicaoProcessoAtivo->tempoDeServico){ 
            trataFimProcesso();
        }else if(tempo_processamento_atual == TAM_QUANTUM){ 
            trataEntradaPreempcao();
        }else{
            processoAtivo->tempoCorrente++;
            tempo_processamento_atual++;
        }
    }

    // Sem processos ativos
    if(processoAtivo == (Processo*) NULL){
        entraProximoProcesso();
        tempo_processamento_atual++;
        return;
    }
}

void printTabelaProcessos(){
    printf("\n== Tabela de Definicoes dos Processos ==\n");

    printf("\n- - - - - - - - - - - - - - - - -");
    for (int i = 0; i < QUANT_PROCESSOS; i++){
        printDefinicaoProcesso(tabelaProcessos[i]);
        printf("- - - - - - - - - - - - - - - - -");
    }
    printf("\n");
}

void printEstadoAtual(){
    printf("\n-- Estado ao Fim do Instante %d --\n\n", tempo_atual);

    if(processoAtivo != (Processo*) NULL)
        printf("Processo %d Ativo: [Tempo de Servico: %d] [Tempo no Quantum: %d]\n", processoAtivo->PID, processoAtivo->tempoCorrente, tempo_processamento_atual);
    else
        printf("Nenhum Processo Ativo! \n");

    if(definicaoProcessoAtivo != (DefinicaoProcesso*) NULL)
        printf("Definicao de Processo %d Ativo: [Tempo Total de Servico: %d]\n", definicaoProcessoAtivo->PID_relacionado, definicaoProcessoAtivo->tempoDeServico);
    else
        printf("Nenhuma Definicao de Processo Ativa!\n");

    if(fila_alta->tam > 0)
        printf("Fila de Alta Prioridade: %d Processos com %d na Frente\n", fila_alta->tam, fila_alta->frente->processo->PID);
    else
        printf("Fila de Alta Prioridade: %d Processos\n", fila_alta->tam);

    if(fila_baixa->tam > 0)
        printf("Fila de Baixa Prioridade: %d Processos com %d na Frente\n", fila_baixa->tam, fila_baixa->frente->processo->PID);
    else
        printf("Fila de Baixa Prioridade: %d Processos\n", fila_baixa->tam);

    if(fila_disco->tam > 0)
        printf("Fila de IO Disco: %d Processos com %d na Frente\n", fila_disco->tam, fila_disco->frente->processo->PID);
    else
        printf("Fila de IO Disco: %d Processos\n", fila_disco->tam);

    if(fila_fita->tam > 0)
        printf("Fila de IO Fita: %d Processos com %d na Frente\n", fila_fita->tam, fila_fita->frente->processo->PID);
    else
        printf("Fila de IO Fita: %d Processos\n", fila_fita->tam);

    if(fila_impressora->tam > 0)
        printf("Fila de IO Impressora: %d Processos com %d na Frente\n", fila_impressora->tam, fila_impressora->frente->processo->PID);
    else
        printf("Fila de IO Impressora: %d Processos\n", fila_impressora->tam);

    printf("\n");
}




// ================ MAIN ================ //

int main(){

    printf("== Iniciando o Escalonador ==\n");

    inicializaEstruturas();

    // Mostra tabela de processos
    printTabelaProcessos();
    
    // Tratamento por Unidade de Tempo    
    while( processos_finalizados < QUANT_PROCESSOS && tempo_atual < 100 ){
        printf("\n== Inicio do Instante %d ==\n", tempo_atual);

        trataNovosProcessos();
        trataSaidasIO();
        trataProcessoAtual();
        printEstadoAtual();
    
        tempo_atual++;
    }
    
    printf("Acabou!!!\n");
    // FREE DA TABELA DE DEFINICAO DE PROCESSOS
    // FREE DAS FILAS
    // FREE DOS PROCESSOS

    return 0;

}