#ifndef INTERFACE
#define INTERFACE

#include <conio.h>
#include <windows.h>

#include "Estruturas.h"
 
HANDLE hout;
HANDLE hin;

CONSOLE_SCREEN_BUFFER_INFO csbi;
COORD coord;

CONSOLE_CURSOR_INFO cci;

INPUT_RECORD InputRecord;
DWORD Events;
DWORD prev_mode;

void definicoesIniciais();

COORD get_console_dimensions(void);

COORD get_console_cursor_pos(void);

void gotoxy(short x, short y);

void puts_centered(char const *str);

void print(char const *str, int *x, int *y);

void println(char const *str, int *x, int *y);

void jumpline(int *x, int *y, int x_inicial, int y_inicial);

void printMemoriaPrincipal(int x_inicial, int y_inicial, Lista *lista);

void printLRUMemoriaPrincipal(int x_inicial, int y_inicial, Lista *lista);

void printTabelaPagina(int x_inicial, int y_inicial, Processo* processo);

void printTabelasPaginas(int x_inicial, int y_inicial, Processo *listaProcessos[NUM_PROCESSOS], int PID, int processosAtivos);

void printSwap(int x_inicial, int y_inicial, Lista* areaDeSwap);

void printTela(Lista *memoriaPrincipal, Processo *listaProcessos[NUM_PROCESSOS], Lista* areaDeSwap, int paginaID, int PID, int processosAtivos);

void proximaSolicitacao(int x_inicial, int y_inicial, int paginaID, int PID);

void aguardaClique();

#endif