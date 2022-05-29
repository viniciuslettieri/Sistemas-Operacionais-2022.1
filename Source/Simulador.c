#include <stdio.h>
#include <stdlib.h>

#include "Util.h"
#include "Processo.h"
#include "ListaIO.h"
#include "Queue.h"

// =============== DECLARACOES =============== //

// Declaração da Tabela de Processos
DefinicaoProcesso *tabelaProcessos[QUANT_PROCESSOS];
Processo *processos[QUANT_PROCESSOS];
int contador_finalizacao = 0;

// Declaração das Filas de Prioridade
Fila *fila_alta;
Fila *fila_baixa;
Fila *fila_disco;
Fila *fila_fita;
Fila *fila_impressora;

// Declaração da Lista de IO
Lista *lista_io;

// Processo em Execução Corrente
Processo *processoAtivo = (Processo *)NULL;
DefinicaoProcesso *definicaoProcessoAtivo = (DefinicaoProcesso *)NULL;

// Informacoes Globais
int tempo_atual = 0;                // tempo desde o inicio do escalonador
int tempo_processamento_atual = 0;  // tempo desde o inicio do quantum atual
int processos_criados = 0;          // quantidade de processos criados ate o momento
int processos_finalizados = 0;      // quantidade de processos finalizados ate o momento

int processos_em_disco = 0;         // quantidade de processos em estado de disco (limite de 1)
int processos_em_fita = 0;          // quantidade de processos em estado de fita (limite de 1)
int processos_em_impressora = 0;    // quantidade de processos em estado de impressora (limite de 1)

// ================= FUNCOES ================= //

void inicializaEstruturas() {
    // Define seed para gerar valores aleatorios
    setaAleatorio();

    // Inicialização da Tabela de Processos
    for (int i = 0; i < QUANT_PROCESSOS; i++) {
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

void trataNovosProcessos() {
    // Novos Processos no Instante Atual sao Inseridos na Fila de Alta Prioridade
    for (int i = 0; i < QUANT_PROCESSOS; i++) {
        if (tabelaProcessos[i] == (DefinicaoProcesso *) NULL) continue;

        if (tabelaProcessos[i]->tempoDeChegada == tempo_atual) {
            Processo *aux = (Processo *) malloc(sizeof(Processo));
            processos[processos_criados] = aux;
            aux->PID = ++processos_criados;
            aux->PPID = 0;
            aux->status = PRONTO;
            aux->tempoCorrente = 0;
            aux->tempoInicio = tempo_atual;

            tabelaProcessos[i]->PID_relacionado = aux->PID;

            insereVerso(fila_alta, aux);

            printf(
                "\n[++] Novo Processo %d Criado! [Relativo ao #%d da Tabela]\n",
                processos_criados, i + 1
            );
        }
    }
}

void entraProximoProcesso() {
    // Encontra o Proximo Processo Pronto para Execucao
    if (fila_alta->tam > 0) {
        processoAtivo = removeFrente(fila_alta);
        processoAtivo->tempoCorrente++;
        processoAtivo->status = EXECUCAO;
        tempo_processamento_atual = 0;
    } else if (fila_baixa->tam > 0) {
        processoAtivo = removeFrente(fila_baixa);
        processoAtivo->tempoCorrente++;
        processoAtivo->status = EXECUCAO;
        tempo_processamento_atual = 0;
    } else {
        printf("\nSem processos prontos!\n");
        return;
    }

    printf("\n[+] Processo %d Iniciou Execucao!\n", processoAtivo->PID);

    // Obtem a definicao do processo em relacao ao PID
    for (int i = 0; i < QUANT_PROCESSOS; i++) {
        if (tabelaProcessos[i] != (DefinicaoProcesso *) NULL &&
            tabelaProcessos[i]->PID_relacionado != -1 &&
            tabelaProcessos[i]->PID_relacionado == processoAtivo->PID) {
                definicaoProcessoAtivo = tabelaProcessos[i];
                return;
        }
    }
}

void trataSaidasIO() {
    // Verifica na lista de IO se algum processo deverá sair no instante atual
    ListaElemento *atual = lista_io->primeiro;
    while (atual != (ListaElemento *)NULL) {
        ListaElemento *prox = atual->proximo;
        if (atual->tempo_saida == tempo_atual) {
            printf("\n[*] Processo %d Saiu de IO!\n", atual->processo->PID);

            atual->processo->status = PRONTO;

            if (atual->tipo_io == IO_DISCO) {
                insereVerso(fila_baixa, atual->processo);
                processos_em_disco--;
            } else if (atual->tipo_io == IO_FITA) {
                insereVerso(fila_alta, atual->processo);
                processos_em_fita--;
            } else if (atual->tipo_io == IO_IMPRESSORA) {
                insereVerso(fila_alta, atual->processo);
                processos_em_impressora--;
            }

            removeElemento(lista_io, atual);
        }
        atual = prox;
    }
}

void trataEntradaFilaIO(enum tipo_io tipo) {
    // Trata para qual fila de IO deve ser inserido o processo corrente
    if (tipo == IO_DISCO) {
        insereVerso(fila_disco, processoAtivo);
        processoAtivo->status = BLOQUEADO;
        printf("\n[-] Processo %d Entrou na Fila de Disco!\n", processoAtivo->PID);
    } else if (tipo == IO_FITA) {
        insereVerso(fila_fita, processoAtivo);
        processoAtivo->status = BLOQUEADO;
        printf("\n[-] Processo %d Entrou na Fila de Fita!\n", processoAtivo->PID);
    } else if (tipo == IO_IMPRESSORA) {
        insereVerso(fila_impressora, processoAtivo);
        processoAtivo->status = BLOQUEADO;
        printf("\n[-] Processo %d Entrou na Fila de Impressora!\n", processoAtivo->PID);
    }

    processoAtivo = (Processo *)NULL;
    definicaoProcessoAtivo = (DefinicaoProcesso *)NULL;
}

void entraProximoIO() {
    // Encontra o Proximo Processo Pronto para iniciar IO
    if (fila_disco->tam > 0 && processos_em_disco == 0) {
        Processo *processoIO = removeFrente(fila_disco);
        insere(lista_io, criaElemento(tempo_atual, tempo_atual + TEMPO_DISCO, IO_DISCO, processoIO));
        processos_em_disco++;
        printf("\n[*] Processo %d Iniciou Disco!\n", processoIO->PID);
    }
    if (fila_fita->tam > 0 && processos_em_fita == 0) {
        Processo *processoIO = removeFrente(fila_fita);
        insere(lista_io, criaElemento(tempo_atual, tempo_atual + TEMPO_FITA, IO_FITA, processoIO));
        processos_em_fita++;
        printf("\n[*] Processo %d Iniciou Fita!\n", processoIO->PID);
    }
    if (fila_impressora->tam > 0 && processos_em_impressora == 0) {
        Processo *processoIO = removeFrente(fila_impressora);
        insere(lista_io, criaElemento(tempo_atual, tempo_atual + TEMPO_IMPRESSORA, IO_IMPRESSORA, processoIO));
        processos_em_impressora++;
        printf("\n[*] Processo %d Iniciou Impressora!\n", processoIO->PID);
    }
}

void trataEntradaPreempcao() {
    printf("\n[-] Processo %d Entrou em Preempcao!\n", processoAtivo->PID);

    processoAtivo->status = PRONTO;
    tempo_processamento_atual = 0;
    insereVerso(fila_baixa, processoAtivo);

    processoAtivo = (Processo *)NULL;
    definicaoProcessoAtivo = (DefinicaoProcesso *)NULL;
}

void trataFimProcesso() {
    printf("\n[--] Processo %d Finalizado!\n", processoAtivo->PID);

    processoAtivo->tempoFinalizacao = tempo_atual;
    processoAtivo->status = SAIDA;

    Processo *aux = processoAtivo;
    processos[contador_finalizacao++] = aux;

    processoAtivo = (Processo *)NULL;
    definicaoProcessoAtivo = (DefinicaoProcesso *)NULL;

    processos_finalizados++;
}

void trataProcessoAtual() {
    // Trata Entrada em IO
    for (int i = 0; i < MAX_IO; i++) {
        if (processoAtivo == (Processo *) NULL) break;

        if (definicaoProcessoAtivo->entradaDisco[i] == processoAtivo->tempoCorrente) {
            trataEntradaFilaIO(IO_DISCO);
        } else if (definicaoProcessoAtivo->entradaFita[i] == processoAtivo->tempoCorrente) {
            trataEntradaFilaIO(IO_FITA);
        } else if (definicaoProcessoAtivo->entradaImpressora[i] == processoAtivo->tempoCorrente) {
            trataEntradaFilaIO(IO_IMPRESSORA);
        }
    }

    // Trata Fim de Processo e Preempcao
    if (processoAtivo != (Processo *) NULL) {
        if (processoAtivo->tempoCorrente ==
            definicaoProcessoAtivo->tempoDeServico) {
            trataFimProcesso();
        } else if (tempo_processamento_atual == TAM_QUANTUM) {
            trataEntradaPreempcao();
        } else {
            processoAtivo->tempoCorrente++;
            tempo_processamento_atual++;
        }
    }

    // Sem processos ativos
    if (processoAtivo == (Processo *)NULL) {
        entraProximoProcesso();
        tempo_processamento_atual++;
    }
}

void printTabelaProcessos() {
    printf("\n== Tabela de Definicoes dos Processos ==\n\n");

    for (int i = 0; i < QUANT_PROCESSOS; i++) {
        printf("- - - - - - - - - - - - - - - - - - - - -");
        printf("\nDefinicao #%d:\n", i + 1);
        printDefinicaoProcesso(tabelaProcessos[i]);
    }
    printf("- - - - - - - - - - - - - - - - - - - - -");
    printf("\n");
}

void printTabelaIO() {
    ListaElemento *atual = lista_io->primeiro;
    while (atual != (ListaElemento *)NULL) {
        printf("[%d: ", atual->processo->PID);

        if (atual->tipo_io == IO_DISCO)
            printf("DISCO");
        else if (atual->tipo_io == IO_FITA)
            printf("FITA");
        else if (atual->tipo_io == IO_IMPRESSORA)
            printf("IMPRESSORA");

        printf(" de %d ate %d] ", atual->tempo_entrada, atual->tempo_saida);
        atual = atual->proximo;
    }
}

void printEstadoAtual() {
    printf("\n-- Estado Antes do Fim do Instante %d --\n\n", tempo_atual);

    if (processoAtivo != (Processo *) NULL)
        printf(
            "Processo %d Ativo: [Tempo de Servico: %d] [Tempo no Quantum: %d]\n",
            processoAtivo->PID, processoAtivo->tempoCorrente,
            tempo_processamento_atual
        );
    else
        printf("Nenhum Processo Ativo! \n");

    if (definicaoProcessoAtivo != (DefinicaoProcesso *)NULL)
        printf("Definicao de Processo %d Ativo: [Tempo Total de Servico: %d]\n",
            definicaoProcessoAtivo->PID_relacionado,
            definicaoProcessoAtivo->tempoDeServico
        );
    else
        printf("Nenhuma Definicao de Processo Ativa!\n");

    if (fila_alta->tam > 0)
        printf(
            "Fila de Alta Prioridade: %d Processos com o processo %d na Frente\n",
            fila_alta->tam, fila_alta->frente->processo->PID
        );
    else
        printf("Fila de Alta Prioridade: %d Processos\n", fila_alta->tam);

    if (fila_baixa->tam > 0)
        printf(
            "Fila de Baixa Prioridade: %d Processos com o processo %d na Frente\n",
            fila_baixa->tam, fila_baixa->frente->processo->PID
        );
    else
        printf("Fila de Baixa Prioridade: %d Processos\n", fila_baixa->tam);

    if (fila_disco->tam > 0)
        printf("Fila de IO Disco: %d Processos com o processo %d na Frente\n",
               fila_disco->tam, fila_disco->frente->processo->PID);
    else
        printf("Fila de IO Disco: %d Processos\n", fila_disco->tam);

    if (fila_fita->tam > 0)
        printf("Fila de IO Fita: %d Processos com o processo %d na Frente\n",
               fila_fita->tam, fila_fita->frente->processo->PID);
    else
        printf("Fila de IO Fita: %d Processos\n", fila_fita->tam);

    if (fila_impressora->tam > 0)
        printf(
            "Fila de IO Impressora: %d Processos com o processo %d na Frente\n",
            fila_impressora->tam, fila_impressora->frente->processo->PID);
    else
        printf("Fila de IO Impressora: %d Processos\n", fila_impressora->tam);

    if (lista_io->size > 0 && lista_io->primeiro != (ListaElemento *)NULL) {
        printf("Lista de IOs em Tratamento: ");
        printTabelaIO();
        printf("\n");
    } else
        printf("Lista de IOs em Tratamento: Vazia\n");

    printf("\n");
}


// ================ MAIN ================ //

int main() {
    printf("== Iniciando o Escalonador ==\n");

    // Inicializa Filas, Lista de IO, Tabela de Definição de Processos
    inicializaEstruturas();

    // Mostra tabela de processos
    printTabelaProcessos();

    // Tratamento por Unidade de Tempo
    while (processos_finalizados < QUANT_PROCESSOS) {
        printf("\n== Inicio do Instante %d ==\n", tempo_atual);

        trataNovosProcessos();
        trataSaidasIO();
        trataProcessoAtual();
        entraProximoIO();

        printEstadoAtual();

        tempo_atual++;
    }

    printf("== Dados dos Processos Finalizados ==\n");
    for (int i = 0; i < QUANT_PROCESSOS; i++) {
        if (processos[i] != (Processo *) NULL)
            printf(
                "Processo %d turnaround:  %d - %d = %d\n", 
                processos[i]->PID,
                processos[i]->tempoFinalizacao,
                processos[i]->tempoInicio,
                processos[i]->tempoFinalizacao - processos[i]->tempoInicio
            );
        else
            printf("nulo\n");
    }

    // Limpeza das Alocacoes
    excluiListaIO(lista_io);
    excluiFila(fila_alta);
    excluiFila(fila_baixa);
    excluiFila(fila_disco);
    excluiFila(fila_fita);
    excluiFila(fila_impressora);

    for (int i = 0; i < QUANT_PROCESSOS; i++) {
        free(processos[i]);
        free(tabelaProcessos[i]);
    }

    printf("\n\n== Termino da Execucao do Escalonador com Sucesso ==\n\n");

    return 0;
}