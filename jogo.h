#ifndef JOGO_H
#define JOGO_H

#include "jogador.h"
#include "tabuleiro.h"

#define NUM_JOGADORES 4

void iniciarJogo(Jogador jogadores[], Tabuleiro *tabuleiro);
void turnoJogador(Jogador *jogador, Tabuleiro *tabuleiro);
int encontrarJogadorComPedraInicial(Jogador jogadores[]);
void jogarPedraInicial(Jogador *jogador, Tabuleiro *tabuleiro);
void removerPedraDaMaoPorIndice(Jogador *jogador, int indice);
void adicionarPecaNoCentro(Tabuleiro *tabuleiro, Pedra *p);
#endif

