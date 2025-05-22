#ifndef JOGADOR_H
#define JOGADOR_H

#include "pedra.h"
#include "tabuleiro.h"

typedef struct jogador {
    char nome[51];
    int pontuacao;
    Pedra *mao;
} Jogador;

void distribuirPecas(Jogador *jogadores, Pedra *todas, Pedra **dorme);
int controlarTurnos(Jogador *jogadores, Tabuleiro *tabuleiro);
Pedra *removerPedraDaMao(Jogador *jogador, int ladoA, int ladoB);
Pedra *selecionarPedraNaMao(Jogador *jogador, Tabuleiro *tabuleiro);

#endif
