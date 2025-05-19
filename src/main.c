#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "jogo.h"
#include <stdio.h>

int main() {
    screenInit(1);
    keyboardInit();
    timerInit(50);

    screenClear();
    screenSetColor(CYAN, BLACK);
    printf("Dominó das ruas\n");
    screenUpdate();

    iniciarJogo();

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}

