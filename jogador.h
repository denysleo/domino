#ifndef JOGADOR_H
#define JOGADOR_H

#include "pedra.h"
#include "tabuleiro.h"
// Forward declaration para evitar inclusão circular se GameState estiver em jogo.h
// e jogo.h incluir jogador.h
typedef struct game_state GameState; 

typedef struct jogador {
    char nome[51];
    int pontuacao;
    Pedra *mao;
} Jogador;

void distribuirPecas(Jogador *jogadores, Pedra *todas, Pedra **dorme);
int controlarTurnos(Jogador *jogadores, Tabuleiro *tabuleiro);
Pedra *removerPedraDaMao(Jogador *jogador, int ladoA, int ladoB);

// Assinatura correta com 3 argumentos e tipo de retorno int
int selecionarPedraNaMao(Jogador *jogador, const GameState *gameState, Pedra **pedraSelecionadaPtr);

void addPedraToMao(Jogador *jogador, Pedra *pedra);
int getHandSize(const Jogador *jogador);
int hasCompatibleMove(const Jogador *jogador, const Tabuleiro *tabuleiro);
int calcularPontosDaMao(const Jogador *jogador); // Declaração necessária

#endif
