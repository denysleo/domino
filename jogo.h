#ifndef JOGO_H
#define JOGO_H

#include "jogador.h"
#include "tabuleiro.h"
#include "pedra.h"

typedef struct game_state {
    Tabuleiro tabuleiro;
    Pedra *dorme;
    Jogador jogadores[4];
    int jogadorAtualIndex;
    int jogadorVencedorIndex;
    int gameOver;

    int lastPlayedSide;
    int lastPlayedByPlayerIndex;
    int lastPlayedWasGato;
    int consecutivePasses;
} GameState;

void iniciarJogo();
int realizarJogada(GameState *gameState);

#endif
