#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdio.h> 
int main() {
    screenInit(1);
    keyboardInit();
    timerInit(51);

    screenClear();
    screenSetColor(CYAN, BLACK);
    printf("Dominó das ruas\n");
    screenUpdate();

    int ch = 0;
    while (ch != 10) { 
        if (keyhit()) {
            ch = readch();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}
