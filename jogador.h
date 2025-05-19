#ifndef JOGADOR_H
#define JOGADOR_H

#include "pedra.h"

typedef struct jogador {
    char nome[51];
    int pontuacao;
    Pedra *mao;
} Jogador;

void distribuirPecas(Jogador *jogadores, Pedra *todas);
void controlarTurnos(Jogador *jogadores);

#endif
