#ifndef JOGADOR_H
#define JOGADOR_H

#include "pedra.h"
#include "tabuleiro.h"

typedef struct game_state GameState;

typedef struct jogador {
    char nome[51];
    int pontuacao;
    Pedra *mao;
} Jogador;

void distribuirPecas(Jogador *jogadores, Pedra *todas, Pedra **dorme);
int controlarTurnos(Jogador *jogadores, Tabuleiro *tabuleiro);
Pedra *removerPedraDaMao(Jogador *jogador, int ladoA, int ladoB);
Pedra *selecionarPedraNaMao(Jogador *jogador, const GameState *gameState);
void addPedraToMao(Jogador *jogador, Pedra *pedra);
int calcularPontosDaMao(const Jogador *jogador);
int getHandSize(const Jogador *jogador);
int hasCompatibleMove(const Jogador *jogador, const Tabuleiro *tabuleiro);

#endif
