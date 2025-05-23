#include "jogador.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tabuleiro.h"
#include "cli-lib/include/screen.h"
#include "cli-lib/include/keyboard.h"
#include "jogo.h"

Pedra *removerPedraDaMao(Jogador *jogador, int ladoA, int ladoB) {
    Pedra *atual = jogador->mao;
    Pedra *anterior = NULL;

    while (atual != NULL) {
        if ((atual->ladoA == ladoA && atual->ladoB == ladoB) ||
            (atual->ladoA == ladoB && atual->ladoB == ladoA)) {

            if (anterior == NULL) {
                jogador->mao = atual->next;
            } else {
                anterior->next = atual->next;
            }
            atual->next = NULL;
            return atual;
        }
        anterior = atual;
        atual = atual->next;
    }
    return NULL;
}

void addPedraToMao(Jogador *jogador, Pedra *pedra) {
    if (pedra == NULL) return;
    pedra->next = jogador->mao;
    jogador->mao = pedra;
}

int controlarTurnos(Jogador *jogadores, Tabuleiro *tabuleiro) {
    int jogadorInicial = -1;
    int maiorCarroca = -1;
    Pedra *pedraInicial = NULL;

    for (int valor = 6; valor >= 0; valor--) {
        for (int i = 0; i < 4; i++) {
            Pedra *m = jogadores[i].mao;
            while (m) {
                if (m->ladoA == valor && m->ladoB == valor) {
                    jogadorInicial = i;
                    maiorCarroca = valor;
                    pedraInicial = removerPedraDaMao(&jogadores[i], valor, valor);
                    goto encontrou;
                }
                m = m->next;
            }
        }
    }

encontrou:
    if (jogadorInicial != -1 && pedraInicial != NULL) {
        printf("Jogador que inicia: %s (com a pedra [%d|%d])\n\n", jogadores[jogadorInicial].nome, maiorCarroca, maiorCarroca);
        adicionarPecaNoCentro(tabuleiro, pedraInicial);
        exibirTabuleiro(tabuleiro);
        return (jogadorInicial + 1) % 4;
    } else {
        printf("Nao foi possivel encontrar a pedra inicial (carroca).\n");
        return -1;
    }
}

void distribuirPecas(Jogador *jogadores, Pedra *todas, Pedra **dorme) {
    Pedra *ptr = todas;

    for (int j = 0; j < 4; j++) {
        jogadores[j].mao = NULL;
        jogadores[j].pontuacao = 0;
        printf("Digite o nome do %d Jogador \n", j + 1);
	    scanf("%s",jogadores[j].nome);
    }

    for (int k = 0; k < 6; k++) {
        for (int j = 0; j < 4; j++) {
            if (!ptr) {
                *dorme = NULL;
                return;
            }
            Pedra *nova = ptr;
            ptr = ptr->next;
            nova->next = jogadores[j].mao;
            jogadores[j].mao = nova;
        }
    }

    *dorme = ptr;
}

Pedra *selecionarPedraNaMao(Jogador *jogador, const GameState *gameState) {
    if (jogador->mao == NULL) {
        printf("Sua mao esta vazia. Nao ha pedras para jogar.\n");
        return NULL;
    }

    int numPedras = 0;
    Pedra *current = jogador->mao;
    while (current != NULL) {
        numPedras++;
        current = current->next;
    }

    Pedra **arrayPedras = (Pedra **)malloc(numPedras * sizeof(Pedra *));
    if (arrayPedras == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria (selecionarPedraNaMao).\n");
        exit(EXIT_FAILURE);
    }

    current = jogador->mao;
    for (int i = 0; i < numPedras; i++) {
        arrayPedras[i] = current;
        current = current->next;
    }

    int selectedIndex = 0;
    int tecla = 0;
    int mudou = 1;

    while (tecla != 10) {
        if (mudou) {
            screenClear();
            screenSetColor(WHITE, BLACK);
            
            printf("Turno de %s\n", jogador->nome);
            printf("Tabuleiro atual: ");
            exibirTabuleiro(&gameState->tabuleiro);
            printf("\n");

            printf("Mao de %s:\n", jogador->nome);
            printf("Use as setas ESQUERDA/DIREITA para selecionar. ENTER para confirmar.\n\n");

            for (int i = 0; i < numPedras; i++) {
                if (i == selectedIndex) {
                    screenSetColor(YELLOW, BLACK);
                } else {
                    screenSetColor(WHITE, BLACK);
                }
                printf("[%d|%d] ", arrayPedras[i]->ladoA, arrayPedras[i]->ladoB);
            }
            printf("\n\n");
            screenUpdate();
            mudou = 0;
        }

        if (keyhit()) {
            tecla = readch();

            if (tecla == 68 && selectedIndex > 0) {
                selectedIndex--;
                mudou = 1;
            } else if (tecla == 67 && selectedIndex < numPedras - 1) {
                selectedIndex++;
                mudou = 1;
            }
        }
    }

    Pedra *pedraSelecionada = arrayPedras[selectedIndex];
    free(arrayPedras);
    return pedraSelecionada;
}

int getHandSize(const Jogador *jogador) {
    int count = 0;
    Pedra *current = jogador->mao;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

int hasCompatibleMove(const Jogador *jogador, const Tabuleiro *tabuleiro) {
    Pedra *current = jogador->mao;
    while (current != NULL) {
        if (isCompatible(current, tabuleiro)) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}
