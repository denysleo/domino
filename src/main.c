#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jogo.h"

int main() {
    screenInit(1);
    keyboardInit();
    timerInit(50);
    srand(time(NULL));

    Jogador jogadores[NUM_JOGADORES];
    Tabuleiro tabuleiro;

    // Inicializar jogadores
    for (int i = 0; i < NUM_JOGADORES; i++) {
        sprintf(jogadores[i].nome, "Jogador %d", i + 1);
        jogadores[i].mao = NULL;
    }

    // Criar todas as pedras e distribuir
    Pedra *todas = criarTodasAsPedras();

    int total_pedras = 28;
    int indices[28];
    for (int i = 0; i < 28; i++) indices[i] = i;

    // Embaralhar índices
    for (int i = 0; i < 28; i++) {
        int r = rand() % 28;
        int temp = indices[i];
        indices[i] = indices[r];
        indices[r] = temp;
    }

    Pedra *array[28];
    Pedra *p = todas;
    for (int i = 0; i < 28; i++) {
        array[i] = p;
        p = p->dir;
        array[i]->dir = NULL;
    }

    int pos = 0;
    for (int j = 0; j < NUM_JOGADORES; j++) {
        for (int c = 0; c < 6; c++) {
            array[pos]->dir = jogadores[j].mao;
            jogadores[j].mao = array[pos];
            pos++;
        }
    }

    iniciarJogo(jogadores, &tabuleiro);

    int ch = 0;
    while (ch != 27) { // ESC para sair
        if (keyhit()) ch = readch();
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}
