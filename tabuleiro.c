#include "tabuleiro.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_PECAS_TABULEIRO 28
#define CAPACIDADE_INICIAL 10

Tabuleiro *criarTabuleiro() {
    Tabuleiro *tabuleiro = malloc(sizeof(Tabuleiro));
    tabuleiro->inicio = NULL;
    tabuleiro->esquerda = NULL;
    tabuleiro->direita = NULL;
    return tabuleiro;
}
void inicializarTabuleiro(Tabuleiro *tabuleiro) {
    tabuleiro->inicio = NULL;
    tabuleiro->esquerda = NULL;
    tabuleiro->direita = NULL;
}

void adicionarPecaNoCentro(Tabuleiro *tabuleiro, Pedra *p) {
    tabuleiro->inicio = p;
    tabuleiro->esquerda = p;
    tabuleiro->direita = p;
    p->esq = NULL;
    p->dir = NULL;
}

void adicionarNaEsquerda(Tabuleiro *tabuleiro, Pedra *p) {
    p->dir = tabuleiro->esquerda;
    if (tabuleiro->esquerda != NULL)
        tabuleiro->esquerda->esq = p;
    tabuleiro->esquerda = p;
    if (tabuleiro->inicio == NULL)
        tabuleiro->inicio = p;
}

void adicionarNaDireita(Tabuleiro *tabuleiro, Pedra *p) {
    p->esq = tabuleiro->direita;
    if (tabuleiro->direita != NULL)
        tabuleiro->direita->dir = p;
    tabuleiro->direita = p;
    if (tabuleiro->inicio == NULL)
        tabuleiro->inicio = p;
}

void exibirTabuleiro(Tabuleiro *tabuleiro) {
    Pedra *atual = tabuleiro->esquerda;
    while (atual) {
        printf("[%d|%d] ", atual->ladoA, atual->ladoB);
        atual = atual->dir;
    }
    printf("\n");
}
