#include "menu.h"
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdio.h>

int menuInicial() {
    int opcao = 1;
    int tecla = 0;

    while (tecla != 10) {  // 10 = ENTER
        screenClear();
        screenSetColor(WHITE, BLACK);
        printf("===== MENU =====\n\n");

        for (int i = 1; i <= 3; i++) {
            if (i == opcao)
                screenSetColor(YELLOW, BLACK);
            else
                screenSetColor(WHITE, BLACK);

            printf("%d. Opção %d\n", i, i);
        }

        screenUpdate();

        if (keyhit()) {
            tecla = readch();

            if (tecla == 65 && opcao > 1) 
                opcao--;
            else if (tecla == 66 && opcao < 3)
                opcao++;
        }
    }

    return opcao;
}


