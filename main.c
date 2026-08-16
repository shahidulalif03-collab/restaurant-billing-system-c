#include "admin.h"
#include "console.h"
#include "customer.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int ycord = 18;
void showWelcomeScreen(void);
void selectionNumInput(int *selectionNum, int xcord, void(retryFunction)(void));

/*-------------------------------
            Main
--------------------------------*/
int main() {
    showWelcomeScreen();
    return 0;
}

/*-------------------------------
        Main Menu
--------------------------------*/
void showWelcomeScreen(void) {
    system("cls");

    int selectionNum;

    gotoxy(30, 2);
    printf("============================================================");

    gotoxy(50, 4);
    printf("RESTAURANT BILLING APP");

    gotoxy(30, 6);
    printf("============================================================");

    gotoxy(47, 7);
    printf("┌────────────────────────────┐");
    gotoxy(47, 8);
    printf("│         WELCOME TO         │");
    gotoxy(47, 9);

    printf("│       THE RESTAURANT       │");
    gotoxy(47, 10);
    printf("└────────────────────────────┘");

    gotoxy(49, 12);
    printf("[ Please Select Your Role ]");

    gotoxy(47, 14);
    printf("[1] 🧑‍ Customer Menu");
    gotoxy(47, 15);
    printf("[2] 🔐 Admin Dashboard");

    gotoxy(30, 17);
    printf("------------------------------------------------------------");

    gotoxy(35, 18);
    printf("➔ Select Option (1-2): ");

    selectionNumInput(&selectionNum, ycord, showWelcomeScreen);

    switch(selectionNum) {
    case 1:
        customerMenu();
        break;

    case 2:
        adminLogin();
        break;

    default:
        gotoxy(30, 20);
        printf("⚠️  Invalid Selection");
        gotoxy(30, 21);
        printf("👉 Press ANY KEY to try again...");
        getch();
        showWelcomeScreen();
    }
}

void selectionNumInput(int *selectionNum, int ycord,
                       void (*retryFunction)(void)) {
    char ch;

    if(scanf("%d", selectionNum) != 1) {
        while((ch = getchar()) != '\n' && ch != EOF)
            ;

        gotoxy(35, ycord + 3);
        printf("⚠️ Invalid Selection");
        gotoxy(35, ycord + 4);
        printf("👉 Press ANY KEY to try again...");
        getch();

        retryFunction();
        return;
    }

    ch = getchar();

    if(ch != '\n') {
        while((ch = getchar()) != '\n' && ch != EOF)
            ;

        gotoxy(35, ycord + 3);
        printf("⚠️ Invalid Selection");
        gotoxy(35, ycord + 4);
        printf("👉 Press ANY KEY to try again...");
        getch();

        retryFunction();
        return;
    }
}

char selectionCharInput(void) {
    char ch, extra;

    if(scanf(" %c", &ch) != 1)
        return '\0';

    extra = getchar();

    if(extra != '\n') {
        while((extra = getchar()) != '\n' && extra != EOF)
            ;
        return '\0';
    }

    return ch;
}