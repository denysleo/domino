#ifndef JOGADOR_H
#define JOGADOR_H

#include "pedra.h"

typedef struct jogador {
    char nome[51];
    int pontuacao;
    Pedra *mao; // lista encadeada de pedras na mão do jogador
} Jogador;

// Distribui as pedras embaralhadas para os 4 jogadores e retorna as pedras do dorme
void distribuirPecas(Jogador *jogadores, Pedra *todas, Pedra **dorme);
void controlarTurnos(Jogador *jogadores);

#endif

