#include <stdio.h>
#include <conio.h>
#include <windows.h>

#include "Interface.h"

#define COR_TITULO 12
#define COR_ENFASE 14
#define COR_PADRAO 15

void definicoesIniciais()
{
    system("cls");

    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    hin = GetStdHandle(STD_INPUT_HANDLE);
    cci.dwSize = 25;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hout, &cci);
    SetConsoleTextAttribute(hout, COR_PADRAO);

    GetConsoleMode(hin, &prev_mode);
    SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));
    // SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
}

COORD get_console_dimensions(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COORD dimensions = {csbi.srWindow.Right - csbi.srWindow.Left,
                        csbi.srWindow.Bottom - csbi.srWindow.Top};
    return dimensions;
}

COORD get_console_cursor_pos(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition;
}

void gotoxy(short x, short y)
{
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void puts_centered(char const *str)
{
    size_t length = strlen(str);
    short x = (short)(get_console_dimensions().X - length) / 2;
    gotoxy(x, get_console_cursor_pos().Y);
    puts(str);
}

void print(char const *str, int *x, int *y)
{
    gotoxy(*x, *y);
    puts(str);
    (*x) += strlen(str);
    gotoxy(*x, *y);
}

void println(char const *str, int *x, int *y)
{
    gotoxy(*x, *y);
    puts(str);
    (*y)++;
    gotoxy(*x, *y);
}

void jumpline(int *x, int *y, int x_inicial, int y_inicial)
{
    gotoxy(*x, *y);
    (*x) = x_inicial;
    (*y)++;
    gotoxy(*x, *y);
}

void printMemoriaPrincipal(int x_inicial, int y_inicial, Fila *fila)
{
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);

    SetConsoleTextAttribute(hout, COR_TITULO);
    print("Frames em Memoria:", &x, &y);
    SetConsoleTextAttribute(hout, COR_PADRAO);
    //jumpline(&x, &y, x_inicial, y_inicial);

    /* FilaElemento *p = fila->primeiro;
    while (p != NULL)
    {
        int frameIndex = p->pagina->frameIndex;
        char str_print[10];

        sprintf(str_print, "%3d", frameIndex);
        x = (x_inicial + 2) + ((3 * (frameIndex)) % (get_console_dimensions().X - 60));
        y = (y_inicial + 2) + ((3 * (frameIndex)) / (get_console_dimensions().X - 60));
        print(str_print, &x, &y);

        p = p->proximo;
    } */
    char str_print[10];
    
    if (fila->size > 1) {
        sprintf(str_print, "%3d  ... %3d", 0, fila->size-1);
    } else {
        sprintf(str_print, "%3d", 0);
    }
    print(str_print, &x, &y);
}

void printLRUMemoriaPrincipal(int x_inicial, int y_inicial, Fila *fila)
{
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);

    SetConsoleTextAttribute(hout, COR_TITULO);
    println("Sequencia LRU: [Frame: Pagina,Processo]", &x, &y);
    SetConsoleTextAttribute(hout, COR_PADRAO);
    jumpline(&x, &y, x_inicial, y_inicial);

    FilaElemento *p = fila->primeiro;
    x = x_inicial + 2;
    int contador = 0;
    while (p != NULL)
    {
        if (x >= get_console_dimensions().X - 20)
        {
            x = x_inicial + 2;
            y++;
        }

        if ( fila->size == fila->tamanhoMaximo && contador == 0)
        {
            SetConsoleTextAttribute(hout, COR_ENFASE);
        }

        char pagina_pid[30];
        sprintf(pagina_pid, "[%d: %d,%d] ",
                p->pagina->frameIndex,
                p->pagina->paginaID,
                p->pagina->PID);

        print(pagina_pid, &x, &y);

        SetConsoleTextAttribute(hout, COR_PADRAO);

        p = p->proximo;
        contador++;
    }
}

void printTabelaPagina(int x_inicial, int y_inicial, Processo *processo)
{
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);

    FilaElemento *p = processo->paginasNaMemoriaPrincipal->primeiro;

    int contador = 0;
    while (p != NULL)
    {
        int paginaID = p->pagina->paginaID;
        int frameIndex = p->pagina->frameIndex;
        char str_print[10];

        if (processo->paginasNaMemoriaPrincipal->size == WORK_SET_LIMIT && contador == 0)
        {
            SetConsoleTextAttribute(hout, COR_ENFASE);
        }

        sprintf(str_print, "%3d", paginaID);
        x = x_inicial + 3 * (paginaID);
        print(str_print, &x, &y);

        SetConsoleTextAttribute(hout, COR_PADRAO);

        p = p->proximo;
        contador++;
    }
}

void printTabelasPaginas(int x_inicial, int y_inicial, Processo *filaProcessos[NUM_PROCESSOS], int PID, int processosAtivos)
{
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);

    SetConsoleTextAttribute(hout, COR_TITULO);
    print("Tabelas de Paginas:", &x, &y);
    SetConsoleTextAttribute(hout, COR_PADRAO);
    //jumpline(&x, &y, x_inicial, y_inicial);

    int x_tabelas = x_inicial + 20;
    gotoxy(x_tabelas, y);
    for (int i = 0; i < NUM_PAGINAS_PROCESSO; i++)
    {
        printf("%3d", i);
    }
    jumpline(&x, &y, x_inicial, y_inicial);
    jumpline(&x, &y, x_inicial, y_inicial);

    // Imprime cada linha da tabela de paginas
    for (int i = 0; i < processosAtivos; i++)
    {
        printf("  Processo %d ", i);
        printTabelaPagina(x_tabelas, y, filaProcessos[i]);
        jumpline(&x, &y, x_inicial, y_inicial);
    }
}

void printSwap(int x_inicial, int y_inicial, Fila *areaDeSwap)
{
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);

    SetConsoleTextAttribute(hout, COR_TITULO);
    println("Area de Swap: [Pagina,Processo]", &x, &y);
    SetConsoleTextAttribute(hout, COR_PADRAO);
    jumpline(&x, &y, x_inicial, y_inicial);

    FilaElemento *p = areaDeSwap->primeiro;
    x = x_inicial + 2;
    while (p != NULL)
    {
        if (x >= get_console_dimensions().X - 20)
        {
            x = x_inicial + 2;
            y++;
        }

        char pagina_pid[30];
        sprintf(pagina_pid, "[%d,%d] ",
                p->pagina->paginaID,
                p->pagina->PID);

        print(pagina_pid, &x, &y);

        p = p->proximo;
    }
}

void proximaSolicitacao(int x_inicial, int y_inicial, int paginaID, int PID)
{
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);

    SetConsoleTextAttribute(hout, COR_TITULO);
    print("Proxima Pagina Requisitada: ", &x, &y);
    SetConsoleTextAttribute(hout, COR_PADRAO);

    char str_print[30];
    sprintf(str_print, "Pagina %d do Processo %d", paginaID, PID);

    SetConsoleTextAttribute(hout, COR_ENFASE);
    print(str_print, &x, &y);
    SetConsoleTextAttribute(hout, COR_PADRAO);
}

void printTela(Fila *memoriaPrincipal, Processo *filaProcessos[NUM_PROCESSOS], Fila *areaDeSwap, int paginaID, int PID, int processosAtivos)
{
    system("cls");
    //puts("\n");
    puts_centered("- Simulador de Memoria -");

    int y_delta = 0;

    if( paginaID != -1 && PID != -1 ){
        proximaSolicitacao(6, y_delta, paginaID, PID);
        y_delta += 2;
    }
    
    printMemoriaPrincipal(6, y_delta, memoriaPrincipal);
    y_delta += 2;
    printLRUMemoriaPrincipal(6, y_delta, memoriaPrincipal);
    y_delta += 8;
    printTabelasPaginas(6, y_delta, filaProcessos, PID, processosAtivos);
    y_delta += 3 + NUM_PROCESSOS;
    printSwap(6, y_delta, areaDeSwap);

    gotoxy(0, 50);
}

void aguardaClique()
{
    SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

    while (1)
    {
        ReadConsoleInput(hin, &InputRecord, 1, &Events);

        if (InputRecord.EventType == MOUSE_EVENT)
        {
            if (InputRecord.Event.MouseEvent.dwEventFlags == 0)
            {
                if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                {
                    break;
                }
            }
        }
    }

    FlushConsoleInputBuffer(hin);
}