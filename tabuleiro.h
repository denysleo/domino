#ifndef TABULEIRO_H
#define TABULEIRO_H

#include "pedra.h"

typedef struct {
    Pedra **pecas;
    int tamanho;
    int capacidade;
} Tabuleiro;

void iniciarTabuleiro(Tabuleiro *tabuleiro);
void adicionarPecaNoCentro(Tabuleiro *tabuleiro, Pedra *p);
void adicionarPecaNoInicio(Tabuleiro *tabuleiro, Pedra *p);
void adicionarPecaNoFim(Tabuleiro *tabuleiro, Pedra *p);
void exibirTabuleiro(const Tabuleiro *tabuleiro);

#endif
