#include <stdio.h>
#include <windows.h>

COORD coord = {0, 0};

void gotoxy(int x, int y) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    // Base design width 120
    int finalX = (width - 120) / 2 + x;

    if(finalX < 0)
        finalX = 0;

    COORD pos = {finalX, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}