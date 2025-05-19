#include "jogador.h"
#include <stdlib.h>
#include <stdio.h>

void distribuirPecas(Jogador *jogadores, Pedra *todas) {
    Pedra *ptr = todas;
    for (int j = 0; j < 4; j++) {
        jogadores[j].mao = NULL;
        jogadores[j].pontuacao = 0;
        sprintf(jogadores[j].nome, "Jogador %d", j + 1);
        for (int k = 0; k < 6; k++) {
            Pedra *nova = ptr;
            ptr = ptr->next;
            nova->next = jogadores[j].mao;
            jogadores[j].mao = nova;
        }
    }
}

void controlarTurnos(Jogador *jogadores) {
    for (int i = 0; i < 4; i++) {
        printf("%s:\n", jogadores[i].nome);
        Pedra *m = jogadores[i].mao;
        while (m) {
            printf("[%d|%d] ", m->ladoA, m->ladoB);
            m = m->next;
        }
        printf("\n\n");
    }
}
