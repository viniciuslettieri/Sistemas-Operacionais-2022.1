#ifndef PROCESSO_H
#define PROCESSO_H

#include "Util.h"

typedef struct _DefinicaoProcesso {
    int PID_relacionado;
    int tempoDeChegada;
    int tempoDeServico;

    // Para os tempos de entrada e saída, -1 representa não participar daquele evento
    int entradaDisco[MAX_IO];
    int entradaFita[MAX_IO];
    int entradaImpressora[MAX_IO];
} DefinicaoProcesso;

typedef struct _Processo {
    int PID;
    int PPID;
    int status;
	int tempoInicio;
	int tempoFinalizacao;
    int tempoCorrente;
} Processo;

void printDefinicaoProcesso(DefinicaoProcesso *dp);

int contemValorEntradas(int valor, DefinicaoProcesso *processo);

DefinicaoProcesso *geraDefinicaoProcesso(int maximoChegada, int maximoServico);

#endif