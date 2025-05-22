#ifndef TABULEIRO_H
#define TABULEIRO_H

#include "pedra.h"

typedef struct tabuleiro {
    Pedra *inicio;
    Pedra *esquerda;
    Pedra *direita;
} Tabuleiro;
Tabuleiro *criarTabuleiro();
void inicializarTabuleiro(Tabuleiro *tabuleiro);
void adicionarNaEsquerda(Tabuleiro *tabuleiro, Pedra *pedra);
void adicionarNaDireita(Tabuleiro *tabuleiro, Pedra *pedra);
void exibirTabuleiro(Tabuleiro *tabuleiro);
void adicionarPecaNoCentro(Tabuleiro *tabuleiro, Pedra *p);
#endif

