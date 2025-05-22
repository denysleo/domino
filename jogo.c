#include "jogo.h"
#include "jogador.h"
#include "pedra.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tabuleiro.h"
#include "cli-lib/include/screen.h"
#include "cli-lib/include/keyboard.h"

int getLadoEsquerdoTabuleiro(const Tabuleiro *tabuleiro) {
    if (tabuleiro->tamanho == 0) return -1;
    return tabuleiro->pecas[0]->ladoA;
}

int getLadoDireitoTabuleiro(const Tabuleiro *tabuleiro) {
    if (tabuleiro->tamanho == 0) return -1;
    return tabuleiro->pecas[tabuleiro->tamanho - 1]->ladoB;
}

int podeJogarPedra(const Pedra *pedra, const Tabuleiro *tabuleiro) {
    return 1;
}

const char* getNomeDupla(int jogadorIndex) {
    if (jogadorIndex == 0 || jogadorIndex == 2) {
        return "Dupla Ímpar (Jogador 1 e Jogador 3)";
    } else {
        return "Dupla Par (Jogador 2 e Jogador 4)";
    }
}

int realizarJogada(Jogador *jogadorAtual, Tabuleiro *tabuleiro, Pedra **dorme) {
    screenClear();
    screenSetColor(WHITE, BLACK);
    printf("Turno de %s\n", jogadorAtual->nome);
    printf("Tabuleiro atual: ");
    exibirTabuleiro(tabuleiro);
    printf("\n");

    Pedra *pedraJogada = NULL;
    int jogadaFeita = 0;
    int jogadorPassou = 0;

    while (!jogadaFeita && !jogadorPassou) {
        if (jogadorAtual->mao == NULL) {
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("Sua mão está vazia. Não há pedras para jogar.\n");
            printf("Pressione ENTER para passar o turno.\n");
            screenUpdate();
            while (readch() != 10);
            jogadorPassou = 1;
            break;
        }

        // CORREÇÃO: Troca de 'seleccionarPedraNaMao' para 'selecionarPedraNaMao'
        pedraJogada = selecionarPedraNaMao(jogadorAtual, tabuleiro);

        if (pedraJogada == NULL) {
            printf("Erro na seleção da pedra.\n");
            break;
        }

        int ladoEscolhido = -1;

        if (tabuleiro->tamanho == 0) {
            printf("Você está colocando a primeira pedra no tabuleiro.\n");
            ladoEscolhido = 1;
        } else {
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("Você escolheu a pedra [%d|%d].\n", pedraJogada->ladoA, pedraJogada->ladoB);
            printf("Em qual lado do tabuleiro você quer jogar?\n");
            printf("1. Esquerdo (lado: %d)\n", getLadoEsquerdoTabuleiro(tabuleiro));
            printf("2. Direito (lado: %d)\n", getLadoDireitoTabuleiro(tabuleiro));
            printf("Use as setas CIMA/BAIXO para escolher. ENTER para confirmar.\n");
            screenUpdate();

            int opcaoLado = 1;
            int mudouLado = 1;
            int teclaLado = 0;

            while (teclaLado != 10) {
                if (mudouLado) {
                    screenClear();
                    screenSetColor(WHITE, BLACK);
                    printf("Você escolheu a pedra [%d|%d].\n", pedraJogada->ladoA, pedraJogada->ladoB);
                    printf("Em qual lado do tabuleiro você quer jogar?\n");

                    if (opcaoLado == 1) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                    printf("1. Esquerdo (lado: %d)\n", getLadoEsquerdoTabuleiro(tabuleiro));

                    if (opcaoLado == 2) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                    printf("2. Direito (lado: %d)\n", getLadoDireitoTabuleiro(tabuleiro));

                    printf("Use as setas CIMA/BAIXO para escolher. ENTER para confirmar.\n");
                    screenUpdate();
                    mudouLado = 0;
                }

                if (keyhit()) {
                    teclaLado = readch();
                    if (teclaLado == 65 && opcaoLado > 1) {
                        opcaoLado--;
                        mudouLado = 1;
                    } else if (teclaLado == 66 && opcaoLado < 2) {
                        opcaoLado++;
                        mudouLado = 1;
                    }
                }
            }
            ladoEscolhido = opcaoLado - 1;
        }

        Pedra *pedraRemovida = removerPedraDaMao(jogadorAtual, pedraJogada->ladoA, pedraJogada->ladoB);
        if (pedraRemovida != NULL) {
            if (ladoEscolhido == 0) {
                adicionarPecaNoInicio(tabuleiro, pedraRemovida);
            } else {
                adicionarPecaNoFim(tabuleiro, pedraRemovida);
            }
            jogadaFeita = 1;
            screenClear();
            screenSetColor(GREEN, BLACK);
            printf("%s jogou a pedra [%d|%d].\n", jogadorAtual->nome, pedraRemovida->ladoA, pedraRemovida->ladoB);
            screenSetColor(WHITE, BLACK);
            exibirTabuleiro(tabuleiro);
            printf("\n");
            screenUpdate();
            printf("Pressione ENTER para continuar...\n");
            while (readch() != 10);

            if (jogadorAtual->mao == NULL) {
                return 1;
            }
        } else {
            screenClear();
            screenSetColor(RED, BLACK);
            printf("Erro ao remover a pedra da mão. Tente novamente.\n");
            screenSetColor(WHITE, BLACK);
            screenUpdate();
            printf("Pressione ENTER para continuar...\n");
            while (readch() != 10);
        }
    }
    return 0;
}

void iniciarJogo() {
    Jogador jogadores[4];
    Pedra *todas = criarTodasAsPedras();
    embaralharListaEncadeada(&todas);
    Pedra *dorme = NULL;
    distribuirPecas(jogadores, todas, &dorme);

    Tabuleiro tabuleiro;
    iniciarTabuleiro(&tabuleiro);

    int jogadorAtualIndex = controlarTurnos(jogadores, &tabuleiro);
    int gameOver = 0;
    int jogadorVencedorIndex = -1;

    if (jogadorAtualIndex != -1) {
        printf("Iniciando o jogo. O primeiro a jogar é o: %s\n\n", jogadores[jogadorAtualIndex].nome);
        while (!gameOver) {
            screenClear();
            printf("Turno do %s:\n", jogadores[jogadorAtualIndex].nome);

            if (realizarJogada(&jogadores[jogadorAtualIndex], &tabuleiro, &dorme)) {
                gameOver = 1;
                jogadorVencedorIndex = jogadorAtualIndex;
            } else {
                jogadorAtualIndex = (jogadorAtualIndex + 1) % 4;
            }
        }

        screenClear();
        screenSetColor(CYAN, BLACK);
        printf("=====================\n");
        printf("===== FIM DE JOGO =====\n");
        printf("=====================\n\n");
        
        if (jogadorVencedorIndex != -1) {
            screenSetColor(YELLOW, BLACK);
            printf("%s zerou a mão!\n", jogadores[jogadorVencedorIndex].nome);
            printf("A dupla vencedora é: %s\n\n", getNomeDupla(jogadorVencedorIndex));
        } else {
            screenSetColor(RED, BLACK);
            printf("O jogo terminou por outro motivo.\n");
        }
        screenSetColor(WHITE, BLACK);
        printf("Pressione ENTER para finalizar...\n");
        screenUpdate();
        while (readch() != 10);
    } else {
        printf("O jogo não pôde ser iniciado corretamente.\n");
        printf("Pressione ENTER para finalizar...\n");
        while (readch() != 10);
    }

    printf("Pedras do dorme:\n");
    Pedra *p = dorme;
    if (p == NULL) {
        printf("[Dorme vazio]\n");
    } else {
        while (p) {
            printf("[%d|%d] ", p->ladoA, p->ladoB);
            p = p->next;
        }
    }
    printf("\n");

    Pedra *temp;
    while(dorme != NULL){
        temp = dorme;
        dorme = dorme->next;
        free(temp);
    }
    // CORREÇÃO: Acesso a membro de struct usando '.' em vez de '->'
    if (tabuleiro.pecas != NULL) {
        free(tabuleiro.pecas);
    }

    for(int i = 0; i < 4; i++){
        Pedra *current_hand = jogadores[i].mao;
        while(current_hand != NULL){
            Pedra *temp_hand = current_hand;
            current_hand = current_hand->next;
            free(temp_hand);
        }
    }
}
