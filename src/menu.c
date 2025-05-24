#include "menu.h"
#include "cli-lib/include/screen.h"
#include "cli-lib/include/keyboard.h"
#include <stdio.h>

#ifndef KEY_ESC
#define KEY_ESC 27
#endif
#ifndef MENU_ACTION_QUIT
#define MENU_ACTION_QUIT 0
#endif

int menuInicial(void) {
    const char *opcoes[] = {
        "Iniciar Jogo",
        "Creditos (Exemplo)",
        "Sair (via Opcao)"
    };
    const int numOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);

    int opcao = 1;
    int tecla = 0;
    int mudou = 1;

    while (1) {
        if (mudou) {
                screenClear();
            screenSetColor(WHITE, BLACK);
            printf("===== MENU DOMINO ===== (ESC para Sair)\n\n");

            for (int i = 0; i < numOpcoes; i++) {
                if ((i + 1) == opcao)
                    screenSetColor(YELLOW, BLACK);
                else
                    screenSetColor(WHITE, BLACK);

                printf("%d. %s\n", i + 1, opcoes[i]);
            }

            screenSetColor(WHITE, BLACK);
            printf("\nUse as setas CIMA/BAIXO e ENTER para selecionar.\n");
            screenUpdate();
            mudou = 0;
        }

        if (keyhit()) {
            tecla = readch();

            if (tecla == 27) {
                int prox1 = readch();
                int prox2 = readch();
                 if (prox1 == 91) {
                    if (prox2 == 65 && opcao > 1) { // seta cima
                        opcao--;
                        mudou = 1;
                    } else if (prox2 == 66 && opcao < numOpcoes) { // seta baixo
                        opcao++;
                        mudou = 1;
                    }
                } else {
                    return MENU_ACTION_QUIT;
                }
            } else if (tecla == KEY_ESC) {
                return MENU_ACTION_QUIT;
            } else if (tecla == 10) {
                break;
            }
        }
    }

    if (opcao == numOpcoes) {
        return MENU_ACTION_QUIT;
    }

    return opcao;
}
