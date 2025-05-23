#include "menu.h"
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdio.h>

int menuInicial() {
    int opcao = 1;
    int tecla = 0;
    int mudou = 1; // flag para redesenhar tela

    while (tecla != 10) {  // 10 = ENTER
        if (mudou) {
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("===== MENU =====\n\n");

            for (int i = 1; i <= 3; i++) {
                if (i == opcao)
                    screenSetColor(YELLOW, BLACK);
                else
                    screenSetColor(WHITE, BLACK);

                if (i == 1)
                    printf("%d. Iniciar Jogo\n", i);
                else
                    printf("%d. Opção %d\n", i, i);
            }

            screenUpdate();
            mudou = 0; // tela atualizada, espera mudança
        }

        if (keyhit()) {
            tecla = readch();

            if (tecla == 65 && opcao > 1) {
                opcao--;
                mudou = 1; // mudou seleção, redesenha
            } else if (tecla == 66 && opcao < 3) {
                opcao++;
                mudou = 1; // mudou seleção, redesenha
            }
        }
    }

    return opcao;
}

