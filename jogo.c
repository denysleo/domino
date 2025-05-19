#include "jogo.h"
#include "jogador.h"
#include "pedra.h"
#include <stdlib.h>

void iniciarJogo() {
    Jogador jogadores[4];
    Pedra *todas = criarTodasAsPedras();
    distribuirPecas(jogadores, todas);
    controlarTurnos(jogadores);
}
