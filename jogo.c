#include <stdio.h>
#include <stdlib.h>
#include "jogo.h"
#include "screen.h"
#include "keyboard.h"

int encontrarJogadorComPedraInicial(Jogador jogadores[]) {
    for (int i = 0; i < NUM_JOGADORES; i++) {
        Pedra *p = jogadores[i].mao;
        while (p != NULL) {
            if ((p->ladoA == 6 && p->ladoB == 6)) {
                return i;
            }
            p = p->dir;
        }
    }
    return -1;
}

void jogarPedraInicial(Jogador *jogador, Tabuleiro *tabuleiro) {
    Pedra *p = jogador->mao;
    Pedra *anterior = NULL;
    while (p != NULL) {
        if (p->ladoA == 6 && p->ladoB == 6) {
            if (anterior == NULL) {
                jogador->mao = p->dir;
            } else {
                anterior->dir = p->dir;
            }
            p->dir = NULL;
            tabuleiro->inicio = p;
            tabuleiro->direita = p;
            return;
        }
        anterior = p;
        p = p->dir;
    }
}

void removerPedraDaMaoPorIndice(Jogador *jogador, int indice) {
    Pedra *atual = jogador->mao;
    Pedra *anterior = NULL;

    int i = 0;
    while (atual != NULL && i < indice) {
        anterior = atual;
        atual = atual->dir;
        i++;
    }

    if (atual == NULL) return;

    if (anterior == NULL) {
        jogador->mao = atual->dir;
    } else {
        anterior->dir = atual->dir;
    }

    free(atual);
}

void turnoJogador(Jogador *jogador, Tabuleiro *tabuleiro) {
    int i = 0;
    Pedra *p = jogador->mao;
    int selecionado = 0;
    int tecla = 0;

    while (tecla != 10) {
        screenClear();
        printf("Jogador: %s\n", jogador->nome);
        printf("Tabuleiro: ");
        exibirTabuleiro(tabuleiro);
        printf("Sua mão:\n");

        p = jogador->mao;
        i = 0;
        while (p != NULL) {
            if (i == selecionado) {
                printf("-> [%d|%d]\n", p->ladoA, p->ladoB);
            } else {
                printf("   [%d|%d]\n", p->ladoA, p->ladoB);
            }
            p = p->dir;
            i++;
        }

        screenUpdate();
        tecla = readch();
        if (tecla == 65 && selecionado > 0) selecionado--;       // seta cima
        if (tecla == 66 && selecionado < i - 1) selecionado++;   // seta baixo
    }

    p = jogador->mao;
    i = 0;
    while (p != NULL && i < selecionado) {
        p = p->dir;
        i++;
    }

    if (p != NULL) {
        adicionarNaDireita(tabuleiro, p);
        removerPedraDaMaoPorIndice(jogador, selecionado);
    }
}

void iniciarJogo(Jogador jogadores[], Tabuleiro *tabuleiro) {
    inicializarTabuleiro(tabuleiro);

    int inicio = encontrarJogadorComPedraInicial(jogadores);
    if (inicio == -1) return;

    jogarPedraInicial(&jogadores[inicio], tabuleiro);

    for (int i = 1; i < NUM_JOGADORES; i++) {
        int j = (inicio + i) % NUM_JOGADORES;
        turnoJogador(&jogadores[j], tabuleiro);
    }
}
