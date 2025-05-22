#include "jogo.h"
#include "jogador.h"
#include "pedra.h"
#include <stdlib.h>
#include <stdio.h>
#include "tabuleiro.h"

void iniciarJogo() {
    Jogador jogadores[4];
    Pedra *todas = criarTodasAsPedras();
    embaralharListaEncadeada(&todas);
    Pedra *dorme = NULL;
    distribuirPecas(jogadores, todas, &dorme);

    Tabuleiro tabuleiro;
    iniciarTabuleiro(&tabuleiro);

    controlarTurnos(jogadores, &tabuleiro);

    printf("Pedras do dorme:\n");
    Pedra *p = dorme;
    while (p) {
        printf("[%d|%d] ", p->ladoA, p->ladoB);
        p = p->next;
    }
    printf("\n");
}


