#ifndef MAIN_H
#define MAIN_H

extern int ycord;
char selectionCharInput(void);
void showWelcomeScreen(void);
void selectionNumInput(int *selectionNum, int ycord,
                       void (*retryFunction)(void));

#endif