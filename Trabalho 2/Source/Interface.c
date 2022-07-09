#include <stdio.h>
#include <conio.h>
#include <windows.h>

#include "Interface.h"
 
void definicoesIniciais(){
	system("cls");

	hout = GetStdHandle(STD_OUTPUT_HANDLE);
    hin = GetStdHandle(STD_INPUT_HANDLE);
    cci.dwSize = 25;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hout, &cci);
    SetConsoleTextAttribute(hout, 15);

	GetConsoleMode(hin, &prev_mode);
    SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));
    //SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
}

COORD get_console_dimensions(void) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COORD dimensions = { csbi.srWindow.Right - csbi.srWindow.Left,
                         csbi.srWindow.Bottom - csbi.srWindow.Top };
    return dimensions;
}

COORD get_console_cursor_pos(void) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition;
}

void gotoxy(short x, short y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void puts_centered(char const *str) {
    size_t length = strlen(str);
    short x = (short)(get_console_dimensions().X - length) / 2;
    gotoxy(x, get_console_cursor_pos().Y);
    puts(str);
}

void print(char const *str, int *x, int *y){
    gotoxy(*x, *y);
    puts(str); 
    (*x) += strlen(str);
    gotoxy(*x, *y);
}

void println(char const *str, int *x, int *y){
    gotoxy(*x, *y);
    puts(str); 
    (*y)++; 
    gotoxy(*x, *y);
}

void jumpline(int *x, int *y, int x_inicial, int y_inicial){
    gotoxy(*x, *y);
    (*x) = x_inicial;
    (*y)++;
    gotoxy(*x, *y);
}

void printMemoriaPrincipal(int x_inicial, int y_inicial, Lista *lista) {
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);
    
    println("Processos em Memoria (Na Sequencia do LRU):", &x, &y);
    jumpline(&x, &y, x_inicial, y_inicial);

    ListaElemento *p = lista->primeiro;
    x = x_inicial+2;
    while (p != NULL){
        if(x >= get_console_dimensions().X - 10){
            x = x_inicial+2;
            y++;
        }

        char pagina_pid[10];
        sprintf(pagina_pid, "(%d, %d)", p->pagina->paginaID, p->pagina->PID);

        print(pagina_pid, &x, &y);
        print(" |", &x, &y);

        p = p->proximo;
    }
}

void printTabelaPagina(int x_inicial, int y_inicial, Processo* processo){
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);

    int contador_wsl = 1;

    for (int i = 0; i < NUM_PAGINAS_PROCESSO; i++){
        if(processo != NULL && processo->tabelaPaginas[i] != NULL){
            int paginaID = processo->tabelaPaginas[i]->pagina->paginaID;
            char str_print[10];

            if(contador_wsl == WORK_SET_LIMIT){
                SetConsoleTextAttribute(hout, 14);
            }
            
            sprintf(str_print, "%3d ", paginaID);
            x = x_inicial + 4*(paginaID);
            print(str_print, &x, &y);
            
            SetConsoleTextAttribute(hout, 15);

            contador_wsl++;
        }
    }
}

void printTabelasPaginas(int x_inicial, int y_inicial, Processo *listaProcessos[NUM_PROCESSOS], int PID, int processosAtivos) {
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);
    
    println("Tabelas de Paginas:", &x, &y);
    jumpline(&x, &y, x_inicial, y_inicial);

    print("  Paginas: ", &x, &y);   
    int x_tabelas = x_inicial+20;
    gotoxy(x_tabelas, y);
    for(int i=0; i<NUM_PAGINAS_PROCESSO; i++){
        printf("%3d ", i);
    }
    jumpline(&x, &y, x_inicial, y_inicial);
    jumpline(&x, &y, x_inicial, y_inicial);

    // Imprime cada linha da tabela de paginas
    for(int i=0; i<processosAtivos; i++){
        printf("  Processo %d ", i);
        printTabelaPagina(x_tabelas, y, listaProcessos[i]);
        jumpline(&x, &y, x_inicial, y_inicial);
    }
}

void printSwap(int x_inicial, int y_inicial) {
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);
    
    println("Area de Swap:", &x, &y);
    jumpline(&x, &y, x_inicial, y_inicial);

    println("  1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10", &x, &y);
}

void proximaSolicitacao(int x_inicial, int y_inicial, int paginaID, int PID){
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);

    print("Proxima Pagina Requisitada: ", &x, &y);

    char str_print[20];
    sprintf(str_print, "Pagina %d do Processo %d", paginaID, PID);

    SetConsoleTextAttribute(hout, 14);
    print(str_print, &x, &y);
    SetConsoleTextAttribute(hout, 15);
}

void printTela(Lista *memoriaPrincipal, Processo *listaProcessos[NUM_PROCESSOS], int paginaID, int PID, int processosAtivos){
    system("cls");
    puts("\n");
    puts_centered("- Simulador de Memoria -");

    int y_delta = 6;
    proximaSolicitacao(6, y_delta, paginaID, PID);
    y_delta += 3;
    printMemoriaPrincipal(6, y_delta, memoriaPrincipal);
    y_delta += 10;
    printTabelasPaginas(6, y_delta, listaProcessos, PID, processosAtivos);
    y_delta += 6 + NUM_PROCESSOS + 1;
    printSwap(6, y_delta);

    gotoxy(0, 50);
}


void aguardaClique(){
    SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

    while(1){
        ReadConsoleInput(hin, &InputRecord, 1, &Events);

        if(InputRecord.EventType == MOUSE_EVENT){
            if(InputRecord.Event.MouseEvent.dwEventFlags == 0){
                if(InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED){
                    break;
                }
            }
        }
    }
    
    FlushConsoleInputBuffer(hin);
}