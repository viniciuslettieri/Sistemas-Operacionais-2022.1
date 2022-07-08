#include <stdio.h>
#include <conio.h>
#include <windows.h>
 
HANDLE hout;
HANDLE hin;

CONSOLE_SCREEN_BUFFER_INFO csbi;
COORD coord;

CONSOLE_CURSOR_INFO cci;

INPUT_RECORD InputRecord;
DWORD Events;
DWORD prev_mode;

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
    puts(str); 
}

void println(char const *str, int *x, int *y){
    puts(str); 
    (*y)++; 
    gotoxy(*x, *y);
}

void jumpline(int *x, int *y, int x_inicial, int y_inicial){
    (*x) = x_inicial;
    (*y)++;
    gotoxy(*x, *y);
}

void printLRU(int x_inicial, int y_inicial) {
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);

    println("Sequencia do LRU Geral:", &x, &y);
    jumpline(&x, &y, x_inicial, y_inicial);

    println("  1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10", &x, &y);
}

void printTabelaPagina(int x_inicial, int y_inicial, int i){
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);

    for(int i=1; i<=50; i++){
        printf("  X ");
    }
}

void printTabelasPaginas(int x_inicial, int y_inicial) {
    int x = x_inicial, y = y_inicial;
    gotoxy(x, y);
    
    println("Tabelas de Paginas:", &x, &y);
    jumpline(&x, &y, x_inicial, y_inicial);

    print("  # Processo", &x, &y);   
    int x_tabelas = x_inicial+20;
    gotoxy(x_tabelas, y);
    for(int i=1; i<=50; i++){
        printf("%3d ", i);
    }
    jumpline(&x, &y, x_inicial, y_inicial);
    jumpline(&x, &y, x_inicial, y_inicial);

    // Imprime cada linha da tabela de paginas
    for(int i=1; i<=20; i++){
        printf("  %d ", i);
        printTabelaPagina(x_tabelas, y, i);
        jumpline(&x, &y, x_inicial, y_inicial);
    }
}

int main(){

    definicoesIniciais();

    puts("\n");
    puts_centered("- Simulador de Memoria -");

    printLRU(6, 6);
    printTabelasPaginas(6, 10);

    gotoxy(0, 50);

    return 0;
}