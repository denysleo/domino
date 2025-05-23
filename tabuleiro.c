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
        if (!tabuleiro->pecas) { fprintf(stderr, "Erro de realocacao de memoria (centro).\n"); exit(EXIT_FAILURE); }
    }
    if (tabuleiro->tamanho == 0) {
        tabuleiro->pecas[0] = p;
        tabuleiro->tamanho = 1;
        p->next = NULL;
    } else {
        fprintf(stderr, "Aviso: adicionarPecaNoCentro so deve ser usada para a primeira peca.\n");
    }
}

void adicionarPecaNoInicio(Tabuleiro *tabuleiro, Pedra *p) {
    if (tabuleiro->tamanho >= tabuleiro->capacidade) {
        tabuleiro->capacidade *= 2;
        tabuleiro->pecas = (Pedra **)realloc(tabuleiro->pecas, sizeof(Pedra *) * tabuleiro->capacidade);
        if (!tabuleiro->pecas) { fprintf(stderr, "Erro de realocacao de memoria (inicio).\n"); exit(EXIT_FAILURE); }
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
        if (!tabuleiro->pecas) { fprintf(stderr, "Erro de realocacao de memoria (fim).\n"); exit(EXIT_FAILURE); }
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

Pedra *removerPecaDoInicio(Tabuleiro *tabuleiro) {
    if (tabuleiro->tamanho == 0) return NULL;

    Pedra *removida = tabuleiro->pecas[0];
    for (int i = 0; i < tabuleiro->tamanho - 1; i++) {
        tabuleiro->pecas[i] = tabuleiro->pecas[i + 1];
    }
    // CORREÇÃO: Nullify the last element after shifting
    tabuleiro->pecas[tabuleiro->tamanho - 1] = NULL; 
    tabuleiro->tamanho--;
    removida->next = NULL;
    return removida;
}

Pedra *removerPecaDoFim(Tabuleiro *tabuleiro) {
    if (tabuleiro->tamanho == 0) return NULL;

    Pedra *removida = tabuleiro->pecas[tabuleiro->tamanho - 1];
    // CORREÇÃO: Nullify the removed element
    tabuleiro->pecas[tabuleiro->tamanho - 1] = NULL; 
    tabuleiro->tamanho--;
    removida->next = NULL;
    return removida;
}

int getLadoEsquerdoTabuleiro(const Tabuleiro *tabuleiro) {
    if (tabuleiro->tamanho == 0) return -1;
    return tabuleiro->pecas[0]->ladoA;
}

int getLadoDireitoTabuleiro(const Tabuleiro *tabuleiro) {
    if (tabuleiro->tamanho == 0) return -1;
    return tabuleiro->pecas[tabuleiro->tamanho - 1]->ladoB;
}

int isCompatible(const Pedra *pedra, const Tabuleiro *tabuleiro) {
    if (tabuleiro->tamanho == 0) return 1;

    int ladoEsquerdo = getLadoEsquerdoTabuleiro(tabuleiro);
    int ladoDireito = getLadoDireitoTabuleiro(tabuleiro);

    if (pedra->ladoA == ladoEsquerdo || pedra->ladoB == ladoEsquerdo) return 1;
    if (pedra->ladoA == ladoDireito || pedra->ladoB == ladoDireito) return 1;

    return 0;
}
