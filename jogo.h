#ifndef JOGO_H
#define JOGO_H

#include "jogador.h"
#include "tabuleiro.h"
#include "pedra.h"

#define KEY_ESC 27

// Status de retorno para funções de jogo e ações de menu
#define ACTION_USER_QUIT -99 // Sinal interno de menuEscolhaPrincipal para ESC

#define STATUS_GAME_CONTINUE 0
#define STATUS_GAME_WIN 1
#define STATUS_GAME_PASS 2
#define STATUS_GAME_QUIT -10


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
int realizarJogada(GameState *gameState); // Retorna status do jogo (WIN, PASS, CONTINUE, QUIT)
void getNomeDupla(int jogadorIndex, Jogador jogadores[4]);

#endif
