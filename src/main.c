#include "cli-lib/include/screen.h"
#include "cli-lib/include/keyboard.h"
#include "cli-lib/include/timer.h"
#include <stdio.h>
#include "menu.h"
#include "jogo.h"

#define MENU_ACTION_QUIT 0
#define MENU_ACTION_START_GAME 1

int main() {
    screenInit(1);
    keyboardInit();
    timerInit(51);

    int escolha = menuInicial();

    screenClear();
    
    if (escolha == MENU_ACTION_QUIT) {
        screenSetColor(WHITE, BLACK);
        printf("Voce escolheu sair. Ate logo!\n");
    } else if (escolha == MENU_ACTION_START_GAME) {
        screenSetColor(CYAN, BLACK);
        iniciarJogo();
    } else if (escolha == 2) {
        screenSetColor(WHITE, BLACK);
        printf("Voce escolheu a segunda opcao (nao implementada).\n");
    } else if (escolha == 3) {
        screenSetColor(WHITE, BLACK);
        printf("Voce escolheu a terceira opcao (nao implementada).\n");
    } else {
        screenSetColor(RED, BLACK);
        printf("Opcao invalida do menu.\n");
    }

    screenUpdate();

    if (escolha != MENU_ACTION_QUIT) {
        printf("\nPressione ENTER para fechar o programa.\n");
        screenUpdate();
        int ch = 0;
        while (ch != 10 && ch != KEY_ESC) { 
            if (keyhit()) {
                ch = readch();
            }
        }
    } else {
        screenUpdate();
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}
