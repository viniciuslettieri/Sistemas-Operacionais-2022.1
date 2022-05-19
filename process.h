#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int numeroDeProcessos = 0;

typedef struct _Processo{
  int numeroDoProcesso;
  int tempoDeChegada;
  int tempoDeServico;

  // Para os tempos de entrada e saída, -1 representa não participar daquele evento
  int tempoDeDisco;
  int tempoDeFita;
  int tempoDeImpressora;
} Processo;
int geraNumero(int minimo, int maximo) {
  srand(time(0));

  return rand()%maximo + minimo;
}

Processo geraProcesso(
    int maximoChegada, 
    int maximoServico, 
    int minimoDisco, 
    int maximoDisco, 
    int minimoFita, 
    int maximoFita, 
    int minimoImpressora,
    int maximoImpressora
  ) {
  Processo processo;
  processo.numeroDoProcesso = numeroDeProcessos++;
  processo.tempoDeChegada = geraNumero(0, maximoChegada);
  processo.tempoDeServico = geraNumero(0, maximoServico);
  processo.tempoDeDisco = geraNumero(minimoDisco, maximoDisco);
  processo.tempoDeFita = geraNumero(minimoFita, maximoFita);
  processo.tempoDeImpressora = geraNumero(minimoImpressora, maximoImpressora);
}