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

int main(){

    definicoesIniciais();

    puts_centered("Hello, World!");
    gotoxy(20, 20);
    puts("Ola Thierry");

    return 0;
}