#include "menu.h"
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdio.h>

int menuInicial() {
    int opcao = 1;
    int tecla = 0;
    int mudou = 1; 

    while (tecla != 10) { 
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
                else if (i == 2)
                    printf("%d. Últimos Vencedores\n", i);
                else
                    printf("%d. Opção %d\n", i, i);
            }

            screenUpdate();
            mudou = 0; 
        }

        if (keyhit()) {
            tecla = readch();

            if (tecla == 65 && opcao > 1) {      
                opcao--;
                mudou = 1;
            } else if (tecla == 66 && opcao < 3) {
                opcao++;
                mudou = 1;
            }
        }
    }

    return opcao;
}

