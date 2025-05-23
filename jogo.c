#include "jogo.h"
#include "jogador.h"
#include "pedra.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tabuleiro.h"
#include "cli-lib/include/screen.h"
#include "cli-lib/include/keyboard.h"

#define OPCAO_REMOVER_GATO 1
#define OPCAO_JOGAR_PEDRA 2

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

int podeJogarPedra(const Pedra *pedra, const Tabuleiro *tabuleiro, int isWinningPlay) {
    if (isWinningPlay) {
        return isCompatible(pedra, tabuleiro);
    }
    return 1;
}

const char* getNomeDupla(int jogadorIndex) {
    if (jogadorIndex == 0 || jogadorIndex == 2) {
        return "Dupla Ímpar (Jogador 1 e Jogador 3)";
    } else {
        return "Dupla Par (Jogador 2 e Jogador 4)";
    }
}

int menuRemoverGatoOuJogar(const GameState *gameState) {
    int opcao = OPCAO_JOGAR_PEDRA;
    int tecla = 0;
    int mudou = 1;

    while (tecla != 10) {
        if (mudou) {
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("Turno de %s\n", gameState->jogadores[gameState->jogadorAtualIndex].nome);
            printf("Tabuleiro atual: ");
            exibirTabuleiro(&gameState->tabuleiro);
            printf("\n");

            printf("Escolha sua acao:\n\n");

            if (gameState->lastPlayedWasGato && gameState->lastPlayedByPlayerIndex != gameState->jogadorAtualIndex) {
                if (opcao == OPCAO_REMOVER_GATO)
                    screenSetColor(YELLOW, BLACK);
                else
                    screenSetColor(WHITE, BLACK);
                printf("%d. Remover Gato (pedra jogada por %s)\n", OPCAO_REMOVER_GATO,
                       gameState->jogadores[gameState->lastPlayedByPlayerIndex].nome);
            } else {
                screenSetColor(DARKGRAY, BLACK);
                printf("1. Remover Gato (Nao disponivel no momento)\n");
            }

            if (opcao == OPCAO_JOGAR_PEDRA)
                screenSetColor(YELLOW, BLACK);
            else
                screenSetColor(WHITE, BLACK);
            printf("%d. Jogar Pedra\n", OPCAO_JOGAR_PEDRA);
            
            screenUpdate();
            mudou = 0;
        }

        if (keyhit()) {
            tecla = readch();

            if (tecla == 65) {
                if (opcao == OPCAO_JOGAR_PEDRA && gameState->lastPlayedWasGato && gameState->lastPlayedByPlayerIndex != gameState->jogadorAtualIndex) {
                    opcao = OPCAO_REMOVER_GATO;
                    mudou = 1;
                }
            } else if (tecla == 66) {
                if (opcao == OPCAO_REMOVER_GATO) {
                    opcao = OPCAO_JOGAR_PEDRA;
                    mudou = 1;
                }
            }
        }
    }
    return opcao;
}

int handlePlayStone(GameState *gameState, Jogador *jogadorAtual) {
    Tabuleiro *tabuleiro = &gameState->tabuleiro;
    Pedra *pedraJogada = NULL;
    int playedThisStone = 0;

    while(!playedThisStone) {
        pedraJogada = selecionarPedraNaMao(jogadorAtual, gameState);

        if (pedraJogada == NULL) {
            screenClear();
            screenSetColor(RED, BLACK);
            printf("Erro na selecao da pedra. Nao foi possivel jogar.\n");
            screenSetColor(WHITE, BLACK);
            screenUpdate();
            printf("Pressione ENTER para continuar...\n");
            while (readch() != 10);
            return 0;
        }

        int isWinningPlay = (jogadorAtual->mao->next == NULL && jogadorAtual->mao == pedraJogada);
        if (!podeJogarPedra(pedraJogada, tabuleiro, isWinningPlay)) {
             screenClear();
             screenSetColor(RED, BLACK);
             printf("Erro: A ultima pedra da mao DEVE ser compativel com o tabuleiro para vencer.\n");
             screenSetColor(WHITE, BLACK);
             screenUpdate();
             printf("Pressione ENTER para tentar novamente...\n");
             while (readch() != 10);
             continue;
        }

        int ladoEscolhido = -1;

        if (tabuleiro->tamanho == 0) {
            printf("Erro interno: Tentando jogar a primeira pedra com tabuleiro vazio de forma inadequada.\n");
            return 0;
        } else {
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("Voce escolheu a pedra [%d|%d].\n", pedraJogada->ladoA, pedraJogada->ladoB);
            printf("Em qual lado do tabuleiro voce quer jogar?\n");
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
                    printf("Voce escolheu a pedra [%d|%d].\n", pedraJogada->ladoA, pedraJogada->ladoB);
                    printf("Em qual lado do tabuleiro voce quer jogar?\n");

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

        Pedra *pedraRemovidaDaMao = removerPedraDaMao(jogadorAtual, pedraJogada->ladoA, pedraJogada->ladoB);
        if (pedraRemovidaDaMao != NULL) {
            int jogadaFoiGato = !isCompatible(pedraRemovidaDaMao, tabuleiro);

            if (ladoEscolhido == 0) {
                if (jogadaFoiGato) {
                    int temp = pedraRemovidaDaMao->ladoA;
                    pedraRemovidaDaMao->ladoA = pedraRemovidaDaMao->ladoB;
                    pedraRemovidaDaMao->ladoB = temp;
                }
                adicionarPecaNoInicio(tabuleiro, pedraRemovidaDaMao);
            } else {
                if (jogadaFoiGato) {
                    int temp = pedraRemovidaDaMao->ladoA;
                    pedraRemovidaDaMao->ladoA = pedraRemovidaDaMao->ladoB;
                    pedraRemovidaDaMao->ladoB = temp;
                }
                adicionarPecaNoFim(tabuleiro, pedraRemovidaDaMao);
            }
            
            playedThisStone = 1;

            gameState->lastPlayedSide = ladoEscolhido;
            gameState->lastPlayedByPlayerIndex = gameState->jogadorAtualIndex;
            gameState->lastPlayedWasGato = jogadaFoiGato;

            screenClear();
            screenSetColor(GREEN, BLACK);
            printf("%s jogou a pedra [%d|%d].\n", jogadorAtual->nome, pedraRemovidaDaMao->ladoA, pedraRemovidaDaMao->ladoB);
            if (jogadaFoiGato) {
                printf("(Foi um GATO!)\n");
            }
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
            printf("Erro ao remover a pedra da mao. Tente novamente.\n");
            screenSetColor(WHITE, BLACK);
            screenUpdate();
            printf("Pressione ENTER para continuar...\n");
            while (readch() != 10);
        }
    }
    return 0;
}


int realizarJogada(GameState *gameState) {
    Jogador *jogadorAtual = &gameState->jogadores[gameState->jogadorAtualIndex];
    Tabuleiro *tabuleiro = &gameState->tabuleiro;

    screenClear();
    screenSetColor(WHITE, BLACK);
    printf("Turno de %s\n", jogadorAtual->nome);
    printf("Tabuleiro atual: ");
    exibirTabuleiro(tabuleiro);
    printf("\n");

    if (jogadorAtual->mao == NULL) {
        screenClear();
        screenSetColor(WHITE, BLACK);
        printf("Sua mao esta vazia. Nao ha pedras para jogar.\n");
        printf("Pressione ENTER para passar o turno.\n");
        screenUpdate();
        while (readch() != 10);
        return 0;
    }

    int choiceHandled = 0;
    while (!choiceHandled) {
        int escolhaAcao = menuRemoverGatoOuJogar(gameState);

        if (escolhaAcao == OPCAO_REMOVER_GATO) {
            if (gameState->lastPlayedWasGato && gameState->lastPlayedByPlayerIndex != gameState->jogadorAtualIndex) {
                Pedra *pedraRemovidaDoTabuleiro = NULL;
                Jogador *jogadorQueJogouGato = &gameState->jogadores[gameState->lastPlayedByPlayerIndex];

                if (gameState->lastPlayedSide == 0) {
                    pedraRemovidaDoTabuleiro = removerPecaDoInicio(tabuleiro);
                } else if (gameState->lastPlayedSide == 1) {
                    pedraRemovidaDoTabuleiro = removerPecaDoFim(tabuleiro);
                }

                if (pedraRemovidaDoTabuleiro != NULL) {
                    addPedraToMao(jogadorQueJogouGato, pedraRemovidaDoTabuleiro);
                    gameState->lastPlayedWasGato = 0;
                    gameState->lastPlayedByPlayerIndex = -1;
                    gameState->lastPlayedSide = -1;

                    screenClear();
                    screenSetColor(GREEN, BLACK);
                    printf("%s removeu a pedra [%d|%d] do tabuleiro.\n",
                           jogadorAtual->nome, pedraRemovidaDoTabuleiro->ladoA, pedraRemovidaDoTabuleiro->ladoB);
                    printf("A pedra voltou para a mao de %s.\n", jogadorQueJogouGato->nome);
                    screenSetColor(WHITE, BLACK);
                    exibirTabuleiro(tabuleiro);
                    printf("\n");
                    screenUpdate();
                    printf("Pressione ENTER para continuar e FAZER SUA JOGADA...\n");
                    while (readch() != 10);
                    choiceHandled = 1;
                } else {
                    screenClear();
                    screenSetColor(RED, BLACK);
                    printf("Erro ao remover o gato do tabuleiro. Tente novamente.\n");
                    screenSetColor(WHITE, BLACK);
                    screenUpdate();
                    printf("Pressione ENTER para continuar e escolher outra acao...\n");
                    while (readch() != 10);
                }
            } else {
                screenClear();
                screenSetColor(RED, BLACK);
                printf("Nao ha um gato valido para remover ou nao e o seu turno de remocao. Escolha outra acao.\n");
                screenSetColor(WHITE, BLACK);
                screenUpdate();
                printf("Pressione ENTER para continuar e escolher outra acao...\n");
                while (readch() != 10);
            }
        } else {
            choiceHandled = 1;
        }
    }

    return handlePlayStone(gameState, jogadorAtual);
}


void iniciarJogo() {
    GameState gameState;

    gameState.jogadorAtualIndex = -1;
    gameState.jogadorVencedorIndex = -1;
    gameState.gameOver = 0;
    gameState.lastPlayedSide = -1;
    gameState.lastPlayedByPlayerIndex = -1;
    gameState.lastPlayedWasGato = 0;
    gameState.consecutivePasses = 0;

    Pedra *todas = criarTodasAsPedras();
    embaralharListaEncadeada(&todas);
    gameState.dorme = NULL;
    distribuirPecas(gameState.jogadores, todas, &gameState.dorme);

    iniciarTabuleiro(&gameState.tabuleiro);

    int primeiroAJogarIndex = controlarTurnos(gameState.jogadores, &gameState.tabuleiro);
    if (primeiroAJogarIndex != -1) {
        gameState.jogadorAtualIndex = primeiroAJogarIndex;
        gameState.lastPlayedSide = 1;
        gameState.lastPlayedByPlayerIndex = (primeiroAJogarIndex + 3) % 4;
        gameState.lastPlayedWasGato = 0;
    } else {
        printf("O jogo nao pode ser iniciado corretamente.\n");
        printf("Pressione ENTER para finalizar...\n");
        while (readch() != 10);
        return;
    }


    printf("Iniciando o jogo. O primeiro a jogar e o: %s\n\n", gameState.jogadores[gameState.jogadorAtualIndex].nome);
    printf("Pressione ENTER para iniciar o primeiro turno...\n");
    while(readch() != 10);


    while (!gameState.gameOver) {
    if (keyhit()) {
        int ch = readch();
        if (ch == 27) {
            if (menuPausa() == 1) {
                gameState.gameOver = 1;
                continue;
            }
        }
    }
    if (realizarJogada(&gameState)) {
        gameState.gameOver = 1;
        gameState.jogadorVencedorIndex = gameState.jogadorAtualIndex;
    } else {
        gameState.jogadorAtualIndex = (gameState.jogadorAtualIndex + 1) % 4;
    }
}


    screenClear();
    screenSetColor(CYAN, BLACK);
    printf("=====================\n");
    printf("===== FIM DE JOGO =====\n");
    printf("=====================\n\n");
    
    if (gameState.jogadorVencedorIndex != -1) {
        screenSetColor(YELLOW, BLACK);
        printf("%s zerou a mao!\n", gameState.jogadores[gameState.jogadorVencedorIndex].nome);
        printf("A dupla vencedora e: %s\n\n", getNomeDupla(gameState.jogadorVencedorIndex));
    } else {
        screenSetColor(RED, BLACK);
        printf("O jogo terminou por outro motivo.\n");
    }
    screenSetColor(WHITE, BLACK);
    printf("Pressione ENTER para finalizar...\n");
    screenUpdate();
    while (readch() != 10);

    Pedra *temp;
    while(gameState.dorme != NULL){
        temp = gameState.dorme;
        gameState.dorme = gameState.dorme->next;
        free(temp);
    }
    if (gameState.tabuleiro.pecas != NULL) {
        free(gameState.tabuleiro.pecas);
    }

    for(int i = 0; i < 4; i++){
        Pedra *current_hand = gameState.jogadores[i].mao;
        while(current_hand != NULL){
            Pedra *temp_hand = current_hand;
            current_hand = current_hand->next;
            free(temp_hand);
        }
    }
}
