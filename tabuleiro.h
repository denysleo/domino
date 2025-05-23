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
Pedra *removerPecaDoInicio(Tabuleiro *tabuleiro);
Pedra *removerPecaDoFim(Tabuleiro *tabuleiro);
int getLadoEsquerdoTabuleiro(const Tabuleiro *tabuleiro);
int getLadoDireitoTabuleiro(const Tabuleiro *tabuleiro);
int isCompatible(const Pedra *pedra, const Tabuleiro *tabuleiro);

#endif
