#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdio.h>
#include "menu.h"
#include "jogo.h"
#include "vencedores.h"

int main() {
    screenInit(1);
    keyboardInit();
    timerInit(51);

    int escolha = menuInicial();

    screenClear();
    screenSetColor(CYAN, BLACK);
    if (escolha == 1) {
        iniciarJogo();
    } else if (escolha == 2) {
       mostrarUltimosVencedores();
    } else if (escolha == 3) {
        printf("Você escolheu a terceira opção\n");
    }

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

