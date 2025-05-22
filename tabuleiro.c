#include "tabuleiro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli-lib/include/screen.h"

#define MAX_PECAS_TABULEIRO 28
#define CAPACIDADE_INICIAL 10

void iniciarTabuleiro(Tabuleiro *tabuleiro) {
    tabuleiro->pecas = (Pedra **)malloc(sizeof(Pedra *) * CAPACIDADE_INICIAL);
    tabuleiro->tamanho = 0;
    tabuleiro->capacidade = CAPACIDADE_INICIAL;
}

void adicionarPecaNoCentro(Tabuleiro *tabuleiro, Pedra *p) {
    if (tabuleiro->tamanho >= tabuleiro->capacidade) {
        tabuleiro->capacidade *= 2;
        tabuleiro->pecas = (Pedra **)realloc(tabuleiro->pecas, sizeof(Pedra *) * tabuleiro->capacidade);
    }
    if (tabuleiro->tamanho == 0) {
        tabuleiro->pecas[0] = p;
        tabuleiro->tamanho = 1;
        p->next = NULL;
    } else {
        fprintf(stderr, "Aviso: adicionarPecaNoCentro só deve ser usada para a primeira peça.\n");
    }
}

void adicionarPecaNoInicio(Tabuleiro *tabuleiro, Pedra *p) {
    if (tabuleiro->tamanho >= tabuleiro->capacidade) {
        tabuleiro->capacidade *= 2;
        tabuleiro->pecas = (Pedra **)realloc(tabuleiro->pecas, sizeof(Pedra *) * tabuleiro->capacidade);
        if (!tabuleiro->pecas) {
            fprintf(stderr, "Erro de alocação de memória ao expandir tabuleiro.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = tabuleiro->tamanho; i > 0; i--) {
        tabuleiro->pecas[i] = tabuleiro->pecas[i - 1];
    }
    tabuleiro->pecas[0] = p;
    tabuleiro->tamanho++;
    p->next = NULL;
}

void adicionarPecaNoFim(Tabuleiro *tabuleiro, Pedra *p) {
    if (tabuleiro->tamanho >= tabuleiro->capacidade) {
        tabuleiro->capacidade *= 2;
        tabuleiro->pecas = (Pedra **)realloc(tabuleiro->pecas, sizeof(Pedra *) * tabuleiro->capacidade);
        if (!tabuleiro->pecas) {
            fprintf(stderr, "Erro de alocação de memória ao expandir tabuleiro.\n");
            exit(EXIT_FAILURE);
        }
    }

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

    for (int i = 0; i < tabuleiro->tamanho; i++) {
        Pedra *p = tabuleiro->pecas[i];
        printf("[%d|%d] ", p->ladoA, p->ladoB);
    }
    printf("\n");
}
