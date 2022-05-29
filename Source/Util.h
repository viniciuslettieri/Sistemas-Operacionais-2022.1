#ifndef UTIL_H
#define UTIL_H

#define MAX_IO 5
#define TEMPO_DISCO 3
#define TEMPO_FITA 5
#define TEMPO_IMPRESSORA 10

#define QUANT_PROCESSOS 3
#define MAX_CHEGADA 15
#define MAX_TEMPO_SERVICO 10
#define TAM_QUANTUM 4

enum estados { NOVO, PRONTO, EXECUCAO, SAIDA, BLOQUEADO };
enum tipo_io { IO_DISCO, IO_FITA, IO_IMPRESSORA };

int geraNumero(int minimo, int maximo);

void setaAleatorio();

#endif