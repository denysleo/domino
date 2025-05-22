#ifndef JOGO_H
#define JOGO_H

#include "jogador.h"
#include "tabuleiro.h"
#include "pedra.h"

void iniciarJogo();
int realizarJogada(Jogador *jogadorAtual, Tabuleiro *tabuleiro, Pedra **dorme);

#endif
