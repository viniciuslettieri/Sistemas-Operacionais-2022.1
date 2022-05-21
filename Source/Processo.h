#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Util.h"

typedef struct _DefinicaoProcesso{
  int tempoDeChegada;
  int tempoDeServico;

  // Para os tempos de entrada e saída, -1 representa não participar daquele evento
  int entradaDisco[MAX_IO];
  int entradaFita[MAX_IO];
  int entradaImpressora[MAX_IO];
} DefinicaoProcesso;

void printDefinicaoProcesso(DefinicaoProcesso* dp);
int contemValorEntradas(int valor, DefinicaoProcesso* processo);

DefinicaoProcesso* geraDefinicaoProcesso(int maximoChegada, int maximoServico) {
  DefinicaoProcesso* processo = (DefinicaoProcesso*) malloc( sizeof(DefinicaoProcesso) );

  processo->tempoDeChegada = geraNumero(0, maximoChegada);
  processo->tempoDeServico = geraNumero(1, maximoServico);

  // Inicializa as entradas com -1
  for(int i=0; i<MAX_IO; i++){
    processo->entradaDisco[i] = -1;
    processo->entradaFita[i] = -1;
    processo->entradaImpressora[i] = -1;
  }

  // Obtem as entradas em IO
  for(int i=0; i<geraNumero(0, MAX_IO); i++) {
    int valor = geraNumero(1, processo->tempoDeServico-1);
    if (!contemValorEntradas(valor, processo))
      processo->entradaDisco[i] = valor;
  } 
  for(int i=0; i<geraNumero(0, MAX_IO); i++){
    int valor = geraNumero(1, processo->tempoDeServico-1);
    if (!contemValorEntradas(valor, processo))
      processo->entradaFita[i] = valor;
  }
  for(int i=0; i<geraNumero(0, MAX_IO); i++) {
    int valor = geraNumero(1, processo->tempoDeServico-1);
    if (!contemValorEntradas(valor, processo))
      processo->entradaImpressora[i] = valor;
  }

  return processo;
}


typedef struct _Processo{
  int PID;
  int PPID;
  int status;
} Processo;

void printDefinicaoProcesso(DefinicaoProcesso* dp){
  printf("Tempo de Chegada: %d\n", dp->tempoDeChegada);
  printf("Tempo de Servico: %d\n\n", dp->tempoDeServico);
  printf("-- Entradas de Disco -- \n");
  for (int i = 0; i < MAX_IO; i++){
    if (dp->entradaDisco[i] != -1)
      printf("Entrada de Disco: %d\n", dp->entradaDisco[i]);
  }
  printf("-- Entradas de Fita -- \n");
  for (int i = 0; i < MAX_IO; i++){
    if (dp->entradaFita[i] != -1)
      printf("Entrada de Fita: %d\n", dp->entradaFita[i]);
  }
  printf("-- Entradas de Impressora -- \n");
  for (int i = 0; i < MAX_IO; i++){
    if (dp->entradaImpressora[i] != -1)
      printf("Entrada de Impressora: %d\n", dp->entradaImpressora[i]);
  }
}

int contemValorEntradas(int valor, DefinicaoProcesso* processo){
  for (int i = 0; i < MAX_IO; i++){
    if (
      processo->entradaDisco[i] == valor || 
      processo->entradaFita[i] == valor || 
      processo->entradaImpressora[i] == valor
      ) 
      return 1;
  }
  return 0;
}