#include "jogo.h"
#include "jogador.h"
#include "pedra.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tabuleiro.h"
#include "cli-lib/include/screen.h"
#include "cli-lib/include/keyboard.h"
#include "cli-lib/include/timer.h" 

#define OPTION_REMOVE_GATO_AND_PLAY 1
#define OPTION_PASS_TURN            2
#define OPTION_PLAY_STONE           3

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
    int currentOptionNumDisplay = 1;
    int maxOptions = 0;

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

    while (tecla != 10 ) { 
        if (mudou) {
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("Turno de %s (ESC para Sair)\n", jogadorAtual->nome);
            printf("Tabuleiro atual: ");
            exibirTabuleiro(tabuleiro);
            printf("\n");

            printf("Sua Mao:\n");
            if (numPedras == 0) {
                printf("[Vazia]\n");
            } else {
                for (int i = 0; i < numPedras; i++) {
                    printf("[%d|%d] ", arrayPedras[i]->ladoA, arrayPedras[i]->ladoB);
                }
                printf("\n");
            }
            printf("\n"); 

            printf("Escolha sua acao:\n\n");
            
            currentOptionNumDisplay = 1;
            maxOptions = 0;

            if (lastWasGatoByOpponent) {
                if (selectedOption == currentOptionNumDisplay) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                printf("%d. Remover Gato (pedra jogada por %s)\n", currentOptionNumDisplay,
                       gameState->jogadores[gameState->lastPlayedByPlayerIndex].nome);
                maxOptions++;
                currentOptionNumDisplay++;
            } else {
                screenSetColor(DARKGRAY, BLACK);
                printf(" . Remover Gato (Nao disponivel)\n");
                screenSetColor(WHITE, BLACK);
            }

            if (!hasCompatible) {
                if (selectedOption == currentOptionNumDisplay) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                printf("%d. Tocar (Passar o turno)\n", currentOptionNumDisplay);
                maxOptions++;
                currentOptionNumDisplay++;

                if (selectedOption == currentOptionNumDisplay) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                printf("%d. Passar Gato (Jogar pedra incompativel)\n", currentOptionNumDisplay);
                maxOptions++;
                currentOptionNumDisplay++;
            } else {
                if (selectedOption == currentOptionNumDisplay) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                printf("%d. Jogar Pedra\n", currentOptionNumDisplay);
                maxOptions++;
                currentOptionNumDisplay++;
            }
            screenSetColor(WHITE, BLACK);

            screenUpdate();
            mudou = 0;
        }

        if (keyhit()) {
            tecla = readch();
            
            // --- INÍCIO DO BLOCO DE DEBUG ---
            fprintf(stderr, "\n\n*** DEBUG: Tecla lida em menuEscolhaPrincipal = %d ***\n", tecla);
            fprintf(stderr, "*** (KEY_ESC = %d, Seta Cima Esperado = 65, Seta Baixo Esperado = 66) ***\n", KEY_ESC);
            fflush(stderr); 
            // Pausa para ver o debug
            // fprintf(stderr, "Pressione qualquer tecla para continuar o debug...\n");
            // fflush(stderr);
            // while(!keyhit()); readch(); // Consome a tecla da pausa
            mudou = 1; 
            // --- FIM DO BLOCO DE DEBUG ---

            if (tecla == KEY_ESC) { 
                 fprintf(stderr, "*** DEBUG: Condicao (tecla == KEY_ESC) VERDADEIRA. Saindo... ***\n");
                 fflush(stderr);
                 if (arrayPedras != NULL) free(arrayPedras);
                 return ACTION_USER_QUIT;
            }

            // Se não for ESC, continua o processamento normal das teclas
            int minSelectableOption = 1;

            if (tecla == 65) { 
                fprintf(stderr, "*** DEBUG: Tecla 65 (SETA CIMA) processada. ***\n");
                fflush(stderr);
                if (selectedOption > minSelectableOption) {
                    selectedOption--;
                }
            } else if (tecla == 66) { 
                fprintf(stderr, "*** DEBUG: Tecla 66 (SETA BAIXO) processada. ***\n");
                fflush(stderr);
                if (selectedOption < maxOptions) {
                    selectedOption++;
                }
            } else if (tecla != 10) { // Se não for ENTER nem uma seta tratada
                fprintf(stderr, "*** DEBUG: Tecla %d NAO tratada para navegacao. Loop continua. ***\n", tecla);
                fflush(stderr);
            }
        }
    }
    
    // Se chegou aqui, tecla == 10 (ENTER)
    fprintf(stderr, "*** DEBUG: Saindo do loop while, tecla = %d (ENTER = 10). Processando escolha. ***\n", tecla);
    fflush(stderr);

    int finalChoice = -1;
    int currentOptionCounter = 1;

    if (lastWasGatoByOpponent) {
        if (selectedOption == currentOptionCounter) {
            finalChoice = OPTION_REMOVE_GATO_AND_PLAY;
        }
        currentOptionCounter++;
    }

    if (finalChoice == -1) { 
        if (!hasCompatible) {
            if (selectedOption == currentOptionCounter) {
                finalChoice = OPTION_PASS_TURN;
            }
            currentOptionCounter++;
            if (selectedOption == currentOptionCounter && finalChoice == -1) {
                finalChoice = OPTION_PLAY_STONE; 
            }
        } else { 
            if (selectedOption == currentOptionCounter) {
                finalChoice = OPTION_PLAY_STONE;
            }
        }
    }
        
    if (arrayPedras != NULL) {
        free(arrayPedras);
    }
    fprintf(stderr, "*** DEBUG: menuEscolhaPrincipal vai retornar: %d ***\n", finalChoice);
    fflush(stderr);
    return finalChoice;
}

int handlePlayStone(GameState *gameState, Jogador *jogadorAtual) {
    Tabuleiro *tabuleiro = &gameState->tabuleiro;
    Pedra *pedraOriginalSelecionada = NULL; 
    Pedra pedraCopiaJogada; 
    int playedThisStone = 0;
    int inter_tecla = 0;

    while(!playedThisStone) {
        int selection_status = selecionarPedraNaMao(jogadorAtual, gameState, &pedraOriginalSelecionada);
        
        if (selection_status == STATUS_GAME_QUIT) {
            fprintf(stderr, "*** DEBUG: handlePlayStone recebeu STATUS_GAME_QUIT de selecionarPedraNaMao ***\n");
            fflush(stderr);
            return STATUS_GAME_QUIT;
        }
        if (pedraOriginalSelecionada == NULL) {
            screenClear();
            screenSetColor(RED, BLACK);
            printf("Selecao de pedra cancelada ou invalida.\n");
            screenSetColor(WHITE, BLACK);
            screenUpdate();
            printf("Pressione ENTER para voltar ao menu de acoes ou ESC para Sair...\n");
            inter_tecla = 0;
            while(inter_tecla != 10 && inter_tecla != KEY_ESC) { 
                if(keyhit()) {
                    inter_tecla = readch();
                }
            }
            if (inter_tecla == KEY_ESC) return STATUS_GAME_QUIT;
            return STATUS_GAME_CONTINUE; 
        }
        
        pedraCopiaJogada.ladoA = pedraOriginalSelecionada->ladoA;
        pedraCopiaJogada.ladoB = pedraOriginalSelecionada->ladoB;

        int querGirar = 0;
        if (pedraCopiaJogada.ladoA != pedraCopiaJogada.ladoB) { 
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("Voce selecionou a pedra [%d|%d]. (ESC para Sair do Jogo)\n", pedraCopiaJogada.ladoA, pedraCopiaJogada.ladoB);
            printf("Deseja girar esta pedra (invertendo os lados)?\n");
            printf("1. Sim (girar para [%d|%d])\n", pedraCopiaJogada.ladoB, pedraCopiaJogada.ladoA);
            printf("2. Nao\n");
            screenUpdate();

            int opcaoGirar = 1; 
            int mudouGirar = 1;
            int teclaGirar = 0;

            while (teclaGirar != 10 && teclaGirar != KEY_ESC) {
                if (mudouGirar) {
                    screenClear();
                    screenSetColor(WHITE, BLACK);
                    printf("Voce selecionou a pedra [%d|%d]. (ESC para Sair do Jogo)\n", pedraCopiaJogada.ladoA, pedraCopiaJogada.ladoB);
                    printf("Deseja girar esta pedra (invertendo os lados)?\n");
                    if (opcaoGirar == 1) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                    printf("1. Sim (girar para [%d|%d])\n", pedraCopiaJogada.ladoB, pedraCopiaJogada.ladoA);
                    if (opcaoGirar == 2) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                    printf("2. Nao\n");
                    printf("Use as setas CIMA/BAIXO para escolher. ENTER para confirmar. ESC para Sair.\n");
                    screenUpdate();
                    mudouGirar = 0;
                }
                if (keyhit()) {
                    teclaGirar = readch();
                    if (teclaGirar == KEY_ESC) break;
                    if (teclaGirar == 65 && opcaoGirar > 1) { opcaoGirar--; mudouGirar = 1; } 
                    else if (teclaGirar == 66 && opcaoGirar < 2) { opcaoGirar++; mudouGirar = 1; }
                }
            }
            if (teclaGirar == KEY_ESC) return STATUS_GAME_QUIT;
            querGirar = (opcaoGirar == 1);
        }

        if (querGirar) {
            int temp = pedraCopiaJogada.ladoA;
            pedraCopiaJogada.ladoA = pedraCopiaJogada.ladoB;
            pedraCopiaJogada.ladoB = temp;
            screenClear();
            screenSetColor(GREEN, BLACK);
            printf("Pedra girada para [%d|%d].\n", pedraCopiaJogada.ladoA, pedraCopiaJogada.ladoB);
            screenSetColor(WHITE, BLACK);
            screenUpdate();
            printf("Pressione ENTER para continuar ou ESC para Sair...\n");
            inter_tecla = 0;
            while(inter_tecla != 10 && inter_tecla != KEY_ESC) { 
                if(keyhit()) {
                    inter_tecla = readch();
                }
            }
            if (inter_tecla == KEY_ESC) return STATUS_GAME_QUIT;
        }
        
        int isWinningPlay = (getHandSize(jogadorAtual) == 1); 
        if (isWinningPlay && !isCompatible(&pedraCopiaJogada, tabuleiro)) {
            screenClear();
            screenSetColor(RED, BLACK);
            printf("Erro: A ultima pedra da mao DEVE ser compativel com o tabuleiro para vencer.\n");
            printf("Pedra [%d|%d] nao e compativel.\n", pedraCopiaJogada.ladoA, pedraCopiaJogada.ladoB);
            screenSetColor(WHITE, BLACK);
            screenUpdate();
            printf("Pressione ENTER para tentar novamente ou ESC para Sair...\n");
            inter_tecla = 0;
            while(inter_tecla != 10 && inter_tecla != KEY_ESC) { 
                if(keyhit()) {
                    inter_tecla = readch();
                }
            }
            if (inter_tecla == KEY_ESC) return STATUS_GAME_QUIT;
            continue; 
        }

        int ladoEscolhido = -1;

        if (tabuleiro->tamanho == 0) {
            Pedra* pRemovida = removerPedraDaMao(jogadorAtual, pedraOriginalSelecionada->ladoA, pedraOriginalSelecionada->ladoB);
            if (pRemovida) {
                pRemovida->ladoA = pedraCopiaJogada.ladoA;
                pRemovida->ladoB = pedraCopiaJogada.ladoB;
                adicionarPecaNoCentro(tabuleiro, pRemovida);
                playedThisStone = 1;
                gameState->lastPlayedSide = 0; 
                gameState->lastPlayedByPlayerIndex = gameState->jogadorAtualIndex;
                gameState->lastPlayedWasGato = 0; 
            } else {
                fprintf(stderr, "Erro crítico: não foi possível remover a pedra selecionada da mão (tabuleiro vazio).\n");
                return STATUS_GAME_CONTINUE;
            }
        } else {
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("Voce escolheu a pedra [%d|%d]. (ESC para Sair do Jogo)\n", pedraCopiaJogada.ladoA, pedraCopiaJogada.ladoB);
            printf("Em qual lado do tabuleiro voce quer jogar?\n");
            printf("1. Esquerdo (lado: %d)\n", getLadoEsquerdoTabuleiro(tabuleiro));
            printf("2. Direito (lado: %d)\n", getLadoDireitoTabuleiro(tabuleiro));
            printf("Use as setas CIMA/BAIXO para escolher. ENTER para confirmar. ESC para Sair.\n");
            screenUpdate();

            int opcaoLado = 1;
            int mudouLado = 1;
            int teclaLado = 0;

            while (teclaLado != 10 && teclaLado != KEY_ESC) {
                if (mudouLado) {
                    screenClear();
                    screenSetColor(WHITE, BLACK);
                    printf("Voce escolheu a pedra [%d|%d]. (ESC para Sair do Jogo)\n", pedraCopiaJogada.ladoA, pedraCopiaJogada.ladoB);
                    printf("Em qual lado do tabuleiro voce quer jogar?\n");
                    if (opcaoLado == 1) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                    printf("1. Esquerdo (lado: %d)\n", getLadoEsquerdoTabuleiro(tabuleiro));
                    if (opcaoLado == 2) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                    printf("2. Direito (lado: %d)\n", getLadoDireitoTabuleiro(tabuleiro));
                    printf("Use as setas CIMA/BAIXO para escolher. ENTER para confirmar. ESC para Sair.\n");
                    screenUpdate();
                    mudouLado = 0;
                }
                if (keyhit()) {
                    teclaLado = readch();
                    if (teclaLado == KEY_ESC) break;
                    if (teclaLado == 65 && opcaoLado > 1) { opcaoLado--; mudouLado = 1;} 
                    else if (teclaLado == 66 && opcaoLado < 2) { opcaoLado++; mudouLado = 1;}
                }
            }
            if (teclaLado == KEY_ESC) return STATUS_GAME_QUIT;
            ladoEscolhido = opcaoLado - 1; 
        
            Pedra *pedraRemovidaDaMao = removerPedraDaMao(jogadorAtual, pedraOriginalSelecionada->ladoA, pedraOriginalSelecionada->ladoB);
            if (pedraRemovidaDaMao != NULL) {
                pedraRemovidaDaMao->ladoA = pedraCopiaJogada.ladoA;
                pedraRemovidaDaMao->ladoB = pedraCopiaJogada.ladoB;
                
                int jogadaFoiGato = !isCompatible(pedraRemovidaDaMao, tabuleiro);

                if (ladoEscolhido == 0) { 
                    adicionarPecaNoInicio(tabuleiro, pedraRemovidaDaMao);
                } else { 
                    adicionarPecaNoFim(tabuleiro, pedraRemovidaDaMao);
                }
                
                playedThisStone = 1;
                gameState->lastPlayedSide = ladoEscolhido;
                gameState->lastPlayedByPlayerIndex = gameState->jogadorAtualIndex;
                gameState->lastPlayedWasGato = jogadaFoiGato;

            } else {
                screenClear();
                screenSetColor(RED, BLACK);
                printf("Erro ao remover a pedra da mao. Tente novamente.\n");
                screenSetColor(WHITE, BLACK);
                screenUpdate();
                printf("Pressione ENTER para continuar ou ESC para Sair...\n");
                inter_tecla = 0;
                while(inter_tecla != 10 && inter_tecla != KEY_ESC) { 
                    if(keyhit()) {
                        inter_tecla = readch();
                    }
                }
                if (inter_tecla == KEY_ESC) return STATUS_GAME_QUIT;
                continue; 
            }
        }

        screenClear();
        screenSetColor(GREEN, BLACK);
        printf("%s jogou a pedra [%d|%d].\n", jogadorAtual->nome, pedraCopiaJogada.ladoA, pedraCopiaJogada.ladoB);
        if (gameState->lastPlayedWasGato) { 
            printf("(Foi um GATO!)\n");
        }
        screenSetColor(WHITE, BLACK);
        exibirTabuleiro(tabuleiro);
        printf("\n");
        screenUpdate();
        printf("Pressione ENTER para continuar ou ESC para Sair...\n");
        inter_tecla = 0;
        while(inter_tecla != 10 && inter_tecla != KEY_ESC) { 
            if(keyhit()) {
                inter_tecla = readch();
            }
        }
        if (inter_tecla == KEY_ESC) return STATUS_GAME_QUIT;

        if (jogadorAtual->mao == NULL) {
            return STATUS_GAME_WIN; 
        }
    }
    return STATUS_GAME_CONTINUE; 
}

int realizarJogada(GameState *gameState) {
    Jogador *jogadorAtual = &gameState->jogadores[gameState->jogadorAtualIndex];
    int finalActionChoice = -1;
    int actionLoopCompleted = 0;
    int inter_tecla = 0; 

    screenClear();
    screenSetColor(WHITE, BLACK);
    printf("Turno de %s (ESC para Sair)\n", jogadorAtual->nome);
    printf("Tabuleiro atual: ");
    exibirTabuleiro(&gameState->tabuleiro);
    printf("\n");

    if (jogadorAtual->mao == NULL) {
        screenClear();
        screenSetColor(WHITE, BLACK);
        printf("Sua mao esta vazia. Nao ha pedras para jogar.\n");
        printf("Pressione ENTER para passar o turno ou ESC para Sair...\n");
        screenUpdate();
        inter_tecla = 0;
        while(inter_tecla != 10 && inter_tecla != KEY_ESC) { if(keyhit()) inter_tecla = readch(); }
        if (inter_tecla == KEY_ESC) return STATUS_GAME_QUIT;
        return STATUS_GAME_PASS; 
    }

    if (getHandSize(jogadorAtual) == 1 && !hasCompatibleMove(jogadorAtual, &gameState->tabuleiro)) {
        screenClear();
        screenSetColor(WHITE, BLACK);
        printf("%s so tem uma pedra ([%d|%d]) e ela nao se encaixa.\n",
               jogadorAtual->nome, jogadorAtual->mao->ladoA, jogadorAtual->mao->ladoB);
        printf("Toca automaticamente.\n");
        screenUpdate();
        printf("Pressione ENTER para passar o turno ou ESC para Sair...\n");
        inter_tecla = 0;
        while(inter_tecla != 10 && inter_tecla != KEY_ESC) { if(keyhit()) inter_tecla = readch(); }
        if (inter_tecla == KEY_ESC) return STATUS_GAME_QUIT;
        return STATUS_GAME_PASS; 
    }

    while (!actionLoopCompleted) {
        finalActionChoice = menuEscolhaPrincipal(gameState);

        if (finalActionChoice == ACTION_USER_QUIT) {
            fprintf(stderr, "*** DEBUG: realizarJogada recebeu ACTION_USER_QUIT de menuEscolhaPrincipal ***\n");
            fflush(stderr);
            return STATUS_GAME_QUIT;
        }

        if (finalActionChoice == OPTION_REMOVE_GATO_AND_PLAY) {
            if (gameState->lastPlayedWasGato && gameState->lastPlayedByPlayerIndex != gameState->jogadorAtualIndex) {
                Pedra *pedraRemovidaDoTabuleiro = NULL;
                Jogador *jogadorQueJogouGato = &gameState->jogadores[gameState->lastPlayedByPlayerIndex];

                if (gameState->lastPlayedSide == 0) { 
                    pedraRemovidaDoTabuleiro = removerPecaDoInicio(&gameState->tabuleiro);
                } else if (gameState->lastPlayedSide == 1) { 
                    pedraRemovidaDoTabuleiro = removerPecaDoFim(&gameState->tabuleiro);
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
                    exibirTabuleiro(&gameState->tabuleiro);
                    printf("\n");
                    screenUpdate();
                    printf("Pressione ENTER para FAZER SUA JOGADA ou ESC para Sair...\n");
                    inter_tecla = 0;
                    while(inter_tecla != 10 && inter_tecla != KEY_ESC) { if(keyhit()) inter_tecla = readch(); }
                    if (inter_tecla == KEY_ESC) return STATUS_GAME_QUIT;
                } else {
                    screenClear();
                    screenSetColor(RED, BLACK);
                    printf("Erro ao remover o gato do tabuleiro. Tente novamente.\n");
                    screenSetColor(WHITE, BLACK);
                    screenUpdate();
                    printf("Pressione ENTER para continuar ou ESC para Sair...\n");
                    inter_tecla = 0;
                    while(inter_tecla != 10 && inter_tecla != KEY_ESC) { if(keyhit()) inter_tecla = readch(); }
                    if (inter_tecla == KEY_ESC) return STATUS_GAME_QUIT;
                }
            } else {
                screenClear();
                screenSetColor(RED, BLACK);
                printf("Nao ha um gato valido para remover. Escolha outra acao.\n");
                screenSetColor(WHITE, BLACK);
                screenUpdate();
                printf("Pressione ENTER para continuar ou ESC para Sair...\n");
                inter_tecla = 0;
                while(inter_tecla != 10 && inter_tecla != KEY_ESC) { if(keyhit()) inter_tecla = readch(); }
                if (inter_tecla == KEY_ESC) return STATUS_GAME_QUIT;
            }
        } else if (finalActionChoice == OPTION_PASS_TURN) {
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("%s tocou (passou o turno).\n", jogadorAtual->nome);
            screenUpdate();
            printf("Pressione ENTER para passar o turno ou ESC para Sair...\n");
            inter_tecla = 0;
            while(inter_tecla != 10 && inter_tecla != KEY_ESC) { if(keyhit()) inter_tecla = readch(); }
            if (inter_tecla == KEY_ESC) return STATUS_GAME_QUIT;
            return STATUS_GAME_PASS; 
        } else if (finalActionChoice == OPTION_PLAY_STONE) {
            actionLoopCompleted = 1; 
        } else {
            screenClear();
            screenSetColor(RED, BLACK);
            printf("Opcao invalida no menu (finalActionChoice = %d). Tente novamente.\n", finalActionChoice);
            screenSetColor(WHITE, BLACK);
            screenUpdate();
            printf("Pressione ENTER para continuar ou ESC para Sair...\n");
            inter_tecla = 0;
            while(inter_tecla != 10 && inter_tecla != KEY_ESC) { if(keyhit()) inter_tecla = readch(); }
            if (inter_tecla == KEY_ESC) return STATUS_GAME_QUIT;
        }
    } 

    int playResult = handlePlayStone(gameState, jogadorAtual);
    return playResult; 
}

void iniciarJogo() {
    GameState gameState;
    int inter_tecla = 0; 

    gameState.jogadorAtualIndex = -1;
    gameState.jogadorVencedorIndex = -1; 
    gameState.gameOver = 0;
    gameState.lastPlayedSide = -1;
    gameState.lastPlayedByPlayerIndex = -1;
    gameState.lastPlayedWasGato = 0;
    gameState.consecutivePasses = 0;

    Pedra *todas = criarTodasAsPedras();
    if (todas == NULL) {
        fprintf(stderr, "Erro ao criar as pedras.\n");
        return;
    }
    embaralharListaEncadeada(&todas);
    gameState.dorme = NULL; 
    distribuirPecas(gameState.jogadores, todas, &gameState.dorme);

    iniciarTabuleiro(&gameState.tabuleiro);

    int primeiroAJogarIndex = controlarTurnos(gameState.jogadores, &gameState.tabuleiro);
    if (primeiroAJogarIndex != -1) {
        gameState.jogadorAtualIndex = primeiroAJogarIndex;
        gameState.lastPlayedSide = 0; 
        gameState.lastPlayedByPlayerIndex = (gameState.jogadorAtualIndex - 1 + 4) % 4; 
        gameState.lastPlayedWasGato = 0; 
    } else {
        printf("O jogo nao pode ser iniciado corretamente (sem carroca inicial).\n");
        printf("Pressione ENTER para finalizar...\n");
        while (readch() != 10);
        Pedra *temp_free_err;
        while(gameState.dorme != NULL){ temp_free_err = gameState.dorme; gameState.dorme = gameState.dorme->next; free(temp_free_err); }
        if (gameState.tabuleiro.pecas != NULL) { 
            for(int i_err = 0; i_err < gameState.tabuleiro.tamanho; i_err++){ if(gameState.tabuleiro.pecas[i_err] != NULL) free(gameState.tabuleiro.pecas[i_err]); }
            free(gameState.tabuleiro.pecas); 
        }
        for(int i_err = 0; i_err < 4; i_err++){ Pedra *current_hand_err = gameState.jogadores[i_err].mao; while(current_hand_err != NULL){ Pedra *temp_hand_err = current_hand_err; current_hand_err = current_hand_err->next; free(temp_hand_err); }}
        return;
    }

    printf("Iniciando o jogo. O proximo a jogar e o: %s\n\n", gameState.jogadores[gameState.jogadorAtualIndex].nome);
    printf("Pressione ENTER para iniciar o primeiro turno ou ESC para Sair...\n");
    inter_tecla = 0;
    while(inter_tecla != 10 && inter_tecla != KEY_ESC) { if(keyhit()) inter_tecla = readch(); }
    if (inter_tecla == KEY_ESC) {
        gameState.gameOver = 1; 
        gameState.jogadorVencedorIndex = -3; 
    }

    while (!gameState.gameOver) {
        int jogadaResultado = realizarJogada(&gameState);

        if (jogadaResultado == STATUS_GAME_QUIT) {
            fprintf(stderr, "*** DEBUG: iniciarJogo recebeu STATUS_GAME_QUIT de realizarJogada ***\n");
            fflush(stderr);
            gameState.gameOver = 1;
            gameState.jogadorVencedorIndex = -3; 
            break; 
        } else if (jogadaResultado == STATUS_GAME_WIN) { 
            gameState.gameOver = 1;
            gameState.jogadorVencedorIndex = gameState.jogadorAtualIndex;
            gameState.consecutivePasses = 0; 
        } else if (jogadaResultado == STATUS_GAME_PASS) { 
            gameState.consecutivePasses++;
            gameState.jogadorAtualIndex = (gameState.jogadorAtualIndex + 1) % 4;
        } else { 
            gameState.consecutivePasses = 0; 
            gameState.jogadorAtualIndex = (gameState.jogadorAtualIndex + 1) % 4;
        }

        if (gameState.consecutivePasses >= 4 && !gameState.gameOver) { 
            gameState.gameOver = 1;
            gameState.jogadorVencedorIndex = -2; 
        }
    }

    screenClear();
    screenSetColor(CYAN, BLACK);
    printf("=====================\n");
    printf("===== FIM DE JOGO =====\n");
    printf("=====================\n\n");
    
    if (gameState.jogadorVencedorIndex == -3) { 
        screenSetColor(WHITE, BLACK);
        printf("Jogo interrompido pelo usuario.\n");
    } else if (gameState.jogadorVencedorIndex == -2) { 
        screenSetColor(YELLOW, BLACK);
        printf("JOGO FECHADO (TRANCADO)!\n");
        printf("Ninguem mais pode jogar. Contando pontos das maos...\n\n");
        screenSetColor(WHITE, BLACK);
        int pontosJogadores[4];
        for (int i = 0; i < 4; i++) {
            pontosJogadores[i] = calcularPontosDaMao(&gameState.jogadores[i]);
            printf("Jogador %s (%s): %d pontos na mao\n",
                   gameState.jogadores[i].nome,
                   (i == 0 || i == 2 ? "Dupla 1" : "Dupla 2"),
                   pontosJogadores[i]);
        }
        printf("\n");
        int pontosDupla1 = pontosJogadores[0] + pontosJogadores[2];
        int pontosDupla2 = pontosJogadores[1] + pontosJogadores[3];
        printf("Total Dupla 1 (%s e %s): %d pontos\n", gameState.jogadores[0].nome, gameState.jogadores[2].nome, pontosDupla1);
        printf("Total Dupla 2 (%s e %s): %d pontos\n", gameState.jogadores[1].nome, gameState.jogadores[3].nome, pontosDupla2);
        printf("\n");
        screenSetColor(YELLOW, BLACK);
        if (pontosDupla1 < pontosDupla2) {
            printf("DUPLA 1 (%s e %s) VENCEU com menos pontos!\n", gameState.jogadores[0].nome, gameState.jogadores[2].nome);
        } else if (pontosDupla2 < pontosDupla1) {
            printf("DUPLA 2 (%s e %s) VENCEU com menos pontos!\n", gameState.jogadores[1].nome, gameState.jogadores[3].nome);
        } else {
            printf("EMPATE entre as duplas na contagem de pontos!\n");
        }
    } else if (gameState.jogadorVencedorIndex != -1) { 
        screenSetColor(YELLOW, BLACK);
        printf("%s zerou a mao!\n", gameState.jogadores[gameState.jogadorVencedorIndex].nome);
        getNomeDupla(gameState.jogadorVencedorIndex, gameState.jogadores);
    } else { 
        screenSetColor(RED, BLACK);
        printf("O jogo terminou por um motivo inesperado (sem vencedor claramente definido).\n");
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
    gameState.dorme = NULL;

    if (gameState.tabuleiro.pecas != NULL) {
        for(int i = 0; i < gameState.tabuleiro.tamanho; i++){
            if(gameState.tabuleiro.pecas[i] != NULL) { 
                free(gameState.tabuleiro.pecas[i]);
                gameState.tabuleiro.pecas[i] = NULL; 
            }
        }
        free(gameState.tabuleiro.pecas);
        gameState.tabuleiro.pecas = NULL; 
    }

    for(int i = 0; i < 4; i++){
        Pedra *current_hand = gameState.jogadores[i].mao;
        while(current_hand != NULL){
            Pedra *temp_hand = current_hand;
            current_hand = current_hand->next;
            free(temp_hand);
        }
        gameState.jogadores[i].mao = NULL; 
    }
}
