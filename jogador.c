#include "jogador.h"
#include <stdlib.h>
#include <stdio.h>

void distribuirPecas(Jogador *jogadores, Pedra *todas, Pedra **dorme) {
    Pedra *ptr = todas;

    // Inicializa jogadores: nome, pontuação e mão (lista vazia)
    for (int j = 0; j < 4; j++) {
        jogadores[j].mao = NULL;
        jogadores[j].pontuacao = 0;
        sprintf(jogadores[j].nome, "Jogador %d", j + 1);
    }

    // Distribui 6 pedras para cada jogador, inserindo na cabeça da lista da mão
    for (int k = 0; k < 6; k++) {
        for (int j = 0; j < 4; j++) {
            if (!ptr) {
                // Caso raro: pedras acabaram antes do esperado
                *dorme = NULL;
                return;
            }
            Pedra *nova = ptr;
            ptr = ptr->next;
            nova->next = jogadores[j].mao;
            jogadores[j].mao = nova;
        }
    }

    // O restante das pedras é o dorme
    *dorme = ptr;
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

