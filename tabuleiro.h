#ifndef TABULEIRO_H
#define TABULEIRO_H

#include "pedra.h"

typedef struct {
    Pedra **pecas;   // Array dinâmico de pedras no tabuleiro
    int tamanho;     // Quantidade de pedras jogadas
    int capacidade;  // Capacidade atual do array
} Tabuleiro;

void iniciarTabuleiro(Tabuleiro *tabuleiro);
void adicionarPecaNoCentro(Tabuleiro *tabuleiro, Pedra *p);
void exibirTabuleiro(const Tabuleiro *tabuleiro);

#endif

