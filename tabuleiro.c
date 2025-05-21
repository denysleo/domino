#include "tabuleiro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli-lib/include/screen.h"  // para screenClear, screenSetColor etc.

#define MAX_PECAS_TABULEIRO 28
#define CAPACIDADE_INICIAL 10


void iniciarTabuleiro(Tabuleiro *tabuleiro) {
    tabuleiro->pecas = (Pedra **)malloc(sizeof(Pedra *) * CAPACIDADE_INICIAL);
    tabuleiro->tamanho = 0;
    tabuleiro->capacidade = CAPACIDADE_INICIAL;
}


void adicionarPecaNoCentro(Tabuleiro *tabuleiro, Pedra *p) {
    // Expande array se necessário
    if (tabuleiro->tamanho >= tabuleiro->capacidade) {
        tabuleiro->capacidade *= 2;
        tabuleiro->pecas = (Pedra **)realloc(tabuleiro->pecas, sizeof(Pedra *) * tabuleiro->capacidade);
    }
    // Inserir pedra no centro (primeira pedra)
    // Como é a primeira, só adiciona
    if (tabuleiro->tamanho == 0) {
        tabuleiro->pecas[0] = p;
        tabuleiro->tamanho = 1;
        p->next = NULL; // garantia de fim de lista
        return;
    }

    // Se já houver pedras, aqui poderia implementar lógica de colocar nas extremidades
    // Por enquanto, vamos apenas adicionar no fim
    tabuleiro->pecas[tabuleiro->tamanho] = p;
    tabuleiro->tamanho++;
    p->next = NULL;
}

void exibirTabuleiro(const Tabuleiro *tabuleiro) {
    printf("Tabuleiro:\n");

    if (tabuleiro->tamanho == 0) {
        printf("[vazio]\n");
        return;
    }

    // Imprime as pedras em linha, centralizadas na tela
    // Para terminal simples, só imprime as pedras com espaços
    for (int i = 0; i < tabuleiro->tamanho; i++) {
        Pedra *p = tabuleiro->pecas[i];
        printf("[%d|%d] ", p->ladoA, p->ladoB);
    }
    printf("\n");
}
