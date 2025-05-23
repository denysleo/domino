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
    int opcao = 1;
    int tecla = 0;
    int mudou = 1; 

    while (tecla != 10 ) { 
        if (tecla == KEY_ESC) {
             return MENU_ACTION_QUIT;
        }
        if (mudou) {
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("===== MENU DOMINO ===== (ESC para Sair)\n\n");

            for (int i = 1; i <= 3; i++) {
                if (i == opcao)
                    screenSetColor(YELLOW, BLACK);
                else
                    screenSetColor(WHITE, BLACK);

                if (i == 1)
                    printf("%d. Iniciar Jogo\n", i);
                else if (i == 2) 
                    printf("%d. Creditos (Exemplo)\n", i);
                else
                    printf("%d. Sair (via Opcao)\n", i); 
            }
            screenSetColor(WHITE, BLACK); 
            printf("\nUse as setas CIMA/BAIXO e ENTER para selecionar.\n");
            screenUpdate();
            mudou = 0; 
        }

        if (keyhit()) {
            tecla = readch();
            if (tecla == KEY_ESC) { 
                 return MENU_ACTION_QUIT;
            }

            if (tecla == 65 && opcao > 1) { 
                opcao--;
                mudou = 1; 
            } else if (tecla == 66 && opcao < 3) { 
                opcao++;
                mudou = 1; 
            }
        }
    }
    
    if (opcao == 3) { 
        return MENU_ACTION_QUIT;
    }

    return opcao; 
}
