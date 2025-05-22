#include "jogador.h"
#include <stdlib.h>
#include <stdio.h>
#include "tabuleiro.h"
#include "jogo.h"
void distribuirPecas(Jogador *jogadores, Pedra *todas, Pedra **dorme) {
    int i = 0, j = 0;
    Pedra *ptr = todas;
    while (ptr != NULL && i < 24) {
        Pedra *nova = criarPedra(ptr->ladoA, ptr->ladoB);
        nova->dir = jogadores[j].mao;
        jogadores[j].mao = nova;
        ptr = ptr->dir;
        i++;
        j = (j + 1) % NUM_JOGADORES;
    }
}

void controlarTurnos(Jogador *jogadores, Tabuleiro *tabuleiro) {
    int jogadorInicial = -1;
    int maiorCarroca = -1;

    for (int valor = 6; valor >= 0; valor--) {
        for (int i = 0; i < 4; i++) {
            Pedra *m = jogadores[i].mao;
            while (m) {
                if (m->ladoA == valor && m->ladoB == valor) {
                    jogadorInicial = i;
                    maiorCarroca = valor;
                    goto encontrou;
                }
                m = m->dir;
            }
        }
    }

encontrou:
    printf("Jogador que inicia: %s (com a pedra [%d|%d])\n\n", jogadores[jogadorInicial].nome, maiorCarroca, maiorCarroca);

    adicionarPecaNoCentro(tabuleiro, criarPedra(maiorCarroca, maiorCarroca));

    exibirTabuleiro(tabuleiro);

    printf("Mão de %s:\n", jogadores[jogadorInicial].nome);
    Pedra *m = jogadores[jogadorInicial].mao;
    while (m) {
        printf("[%d|%d] ", m->ladoA, m->ladoB);
        m = m->dir;
    }
    printf("\n\n");
}

