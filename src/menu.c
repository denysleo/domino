#include <stdio.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "menu.h"

int menuInicial(void) {
    int opcao = 0;

    while (opcao < 1 || opcao > 3) {
        screenClear();
        screenSetColor(CYAN, BLACK);
        printf("=== DOMINÓ DAS RUAS ===\n");
        printf("1. Iniciar Jogo\n");
        printf("2. Instruções\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        screenUpdate();

        while (!keyhit()) {
            timerSleep(10);
        }

        int ch = readch();
        opcao = ch - '0';

        if (opcao < 1 || opcao > 3) {
            printf("\nOpção inválida! Tente novamente.\n");
            timerSleep(1000);
        }
    }

    return opcao;
}

