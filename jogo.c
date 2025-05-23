#include "jogo.h"
#include "jogador.h"
#include "pedra.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tabuleiro.h"
#include "cli-lib/include/screen.h"
#include "cli-lib/include/keyboard.h"

#define OPTION_REMOVE_GATO_AND_PLAY 1
#define OPTION_PASS_TURN            2
#define OPTION_PLAY_STONE           3

int podeJogarPedra(const Pedra *pedra, const Tabuleiro *tabuleiro, int isWinningPlay) {
    if (isWinningPlay) {
        return isCompatible(pedra, tabuleiro);
    }
    return 1;
}

void getNomeDupla(int jogadorIndex, Jogador jogadores[4]) {
    if (jogadorIndex == 0 || jogadorIndex == 2) {
        printf("%s e %s VENCERAM!\n", jogadores[0].nome, jogadores[2].nome);
    } else {
        printf("%s e %s VENCERAM!\n", jogadores[1].nome, jogadores[3].nome);
    }
}

int menuEscolhaPrincipal(const GameState *gameState) {
    const Jogador *jogadorAtual = &gameState->jogadores[gameState->jogadorAtualIndex];
    const Tabuleiro *tabuleiro = &gameState->tabuleiro;
    
    int hasCompatible = hasCompatibleMove(jogadorAtual, tabuleiro);
    int lastWasGatoByOpponent = (gameState->lastPlayedWasGato && gameState->lastPlayedByPlayerIndex != gameState->jogadorAtualIndex);

    int selectedOption = 1;
    int tecla = 0;
    int mudou = 1;
    int currentOptionNum = 1;

    // Obter as pedras da mão do jogador para exibição
    int numPedras = 0;
    Pedra *current = jogadorAtual->mao;
    while (current != NULL) {
        numPedras++;
        current = current->next;
    }

    Pedra **arrayPedras = NULL;
    if (numPedras > 0) {
        arrayPedras = (Pedra **)malloc(numPedras * sizeof(Pedra *));
        if (arrayPedras == NULL) {
            fprintf(stderr, "Erro de alocacao de memoria (menuEscolhaPrincipal).\n");
            exit(EXIT_FAILURE);
        }
        current = jogadorAtual->mao;
        for (int i = 0; i < numPedras; i++) {
            arrayPedras[i] = current;
            current = current->next;
        }
    }


    while (tecla != 10) {
        if (mudou) {
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("Turno de %s\n", jogadorAtual->nome);
            printf("Tabuleiro atual: ");
            exibirTabuleiro(tabuleiro);
            printf("\n");

            // Exibir a mão do jogador aqui
            printf("Sua Mao:\n");
            if (numPedras == 0) {
                printf("[Vazia]\n");
            } else {
                for (int i = 0; i < numPedras; i++) {
                    printf("[%d|%d] ", arrayPedras[i]->ladoA, arrayPedras[i]->ladoB);
                }
                printf("\n");
            }
            printf("\n"); // Espaçamento

            printf("Escolha sua acao:\n\n");

            currentOptionNum = 1;

            if (lastWasGatoByOpponent) {
                if (selectedOption == currentOptionNum) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                printf("%d. Remover Gato (pedra jogada por %s)\n", currentOptionNum,
                       gameState->jogadores[gameState->lastPlayedByPlayerIndex].nome);
                currentOptionNum++;
            } else {
                screenSetColor(DARKGRAY, BLACK);
                printf(" . Remover Gato (Nao disponivel)\n");
            }
            screenSetColor(WHITE, BLACK);

            if (!hasCompatible) {
                if (selectedOption == currentOptionNum) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                printf("%d. Tocar (Passar o turno)\n", currentOptionNum);
                currentOptionNum++;
                if (selectedOption == currentOptionNum) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                printf("%d. Passar Gato (Jogar pedra incompativel)\n", currentOptionNum);
                currentOptionNum++;
            } else {
                if (selectedOption == currentOptionNum) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                printf("%d. Jogar Pedra\n", currentOptionNum);
                currentOptionNum++;
            }
            screenSetColor(WHITE, BLACK);

            screenUpdate();
            mudou = 0;
        }

        if (keyhit()) {
            tecla = readch();

            if (tecla == 65) {
                if (selectedOption > 1) {
                    selectedOption--;
                    mudou = 1;
                }
            } else if (tecla == 66) {
                if (selectedOption < currentOptionNum - 1) {
                    selectedOption++;
                    mudou = 1;
                }
            }
        }
    }

    int finalChoice = -1;

    int optionOffset = 0;
    if (lastWasGatoByOpponent) {
        if (selectedOption == 1) {
            finalChoice = OPTION_REMOVE_GATO_AND_PLAY;
        } else {
            optionOffset = 1;
        }
    }

    if (!hasCompatible) {
        if (selectedOption == (1 + optionOffset)) {
            finalChoice = OPTION_PASS_TURN;
        } else if (selectedOption == (2 + optionOffset)) {
            finalChoice = OPTION_PLAY_STONE;
        }
    } else {
        if (selectedOption == (1 + optionOffset)) {
            finalChoice = OPTION_PLAY_STONE;
        }
    }
    
    // Liberar a memória alocada para arrayPedras
    if (arrayPedras != NULL) {
        free(arrayPedras);
    }

    return finalChoice;
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
        if (isWinningPlay && !isCompatible(pedraJogada, tabuleiro)) {
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

    if (getHandSize(jogadorAtual) == 1 && !hasCompatibleMove(jogadorAtual, tabuleiro)) {
        screenClear();
        screenSetColor(WHITE, BLACK);
        printf("%s so tem uma pedra ([%d|%d]) e ela nao se encaixa.\n",
               jogadorAtual->nome, jogadorAtual->mao->ladoA, jogadorAtual->mao->ladoB);
        printf("Toca automaticamente.\n");
        screenUpdate();
        printf("Pressione ENTER para passar o turno...\n");
        while (readch() != 10);
        return 0;
    }

    int finalActionChoice = -1;
    int actionExecutedSuccessfully = 0;

    while (!actionExecutedSuccessfully) {
        finalActionChoice = menuEscolhaPrincipal(gameState);

        if (finalActionChoice == OPTION_REMOVE_GATO_AND_PLAY) {
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
                    actionExecutedSuccessfully = 1;
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
                printf("Nao ha um gato valido para remover. Escolha outra acao.\n");
                screenSetColor(WHITE, BLACK);
                screenUpdate();
                printf("Pressione ENTER para continuar e escolher outra acao...\n");
                while (readch() != 10);
            }
        } else if (finalActionChoice == OPTION_PASS_TURN) {
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("%s tocou (passou o turno).\n", jogadorAtual->nome);
            screenUpdate();
            printf("Pressione ENTER para passar o turno...\n");
            while (readch() != 10);
            return 0;
        } else if (finalActionChoice == OPTION_PLAY_STONE) {
            actionExecutedSuccessfully = 1;
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
        getNomeDupla(gameState.jogadorVencedorIndex, gameState.jogadores);
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
