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
  for(int i=0; i<geraNumero(0, MAX_IO); i++)
    processo->entradaDisco[i] = geraNumero(1, processo->tempoDeServico-1);
  for(int i=0; i<geraNumero(0, MAX_IO); i++)
    processo->entradaFita[i] = geraNumero(1, processo->tempoDeServico-1);
  for(int i=0; i<geraNumero(0, MAX_IO); i++)
    processo->entradaImpressora[i] = geraNumero(1, processo->tempoDeServico-1);

  return processo;
}


typedef struct _Processo{
  int PID;
  int PPID;
  int status;
} Processo;