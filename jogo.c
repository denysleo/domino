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

// Declaração da função que deve estar em jogador.h e implementada em jogador.c
// int calcularPontosDaMao(const Jogador *jogador);

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

    if (finalChoice == -1) { // Only proceed if not already set to REMOVE_GATO
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
    }
    
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

        int querGirar = 0;
        if (pedraJogada->ladoA != pedraJogada->ladoB) { 
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("Voce selecionou a pedra [%d|%d].\n", pedraJogada->ladoA, pedraJogada->ladoB);
            printf("Deseja girar esta pedra (invertendo os lados)?\n");
            printf("1. Sim (girar para [%d|%d])\n", pedraJogada->ladoB, pedraJogada->ladoA);
            printf("2. Nao\n");
            screenUpdate();

            int opcaoGirar = 1; 
            int mudouGirar = 1;
            int teclaGirar = 0;

            while (teclaGirar != 10) {
                if (mudouGirar) {
                    screenClear();
                    screenSetColor(WHITE, BLACK);
                    printf("Voce selecionou a pedra [%d|%d].\n", pedraJogada->ladoA, pedraJogada->ladoB);
                    printf("Deseja girar esta pedra (invertendo os lados)?\n");

                    if (opcaoGirar == 1) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                    printf("1. Sim (girar para [%d|%d])\n", pedraJogada->ladoB, pedraJogada->ladoA);

                    if (opcaoGirar == 2) screenSetColor(YELLOW, BLACK); else screenSetColor(WHITE, BLACK);
                    printf("2. Nao\n");

                    printf("Use as setas CIMA/BAIXO para escolher. ENTER para confirmar.\n");
                    screenUpdate();
                    mudouGirar = 0;
                }

                if (keyhit()) {
                    teclaGirar = readch();
                    if (teclaGirar == 65 && opcaoGirar > 1) { 
                        opcaoGirar--;
                        mudouGirar = 1;
                    } else if (teclaGirar == 66 && opcaoGirar < 2) { 
                        opcaoGirar++;
                        mudouGirar = 1;
                    }
                }
            }
            querGirar = (opcaoGirar == 1);
        }

        if (querGirar) {
            int temp = pedraJogada->ladoA;
            pedraJogada->ladoA = pedraJogada->ladoB;
            pedraJogada->ladoB = temp;
            screenClear();
            screenSetColor(GREEN, BLACK);
            printf("Pedra girada para [%d|%d].\n", pedraJogada->ladoA, pedraJogada->ladoB);
            screenSetColor(WHITE, BLACK);
            screenUpdate();
            printf("Pressione ENTER para continuar...\n");
            while (readch() != 10);
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
            if(querGirar){ // Desfaz o giro se a jogada vencedora for inválida
                int temp = pedraJogada->ladoA;
                pedraJogada->ladoA = pedraJogada->ladoB;
                pedraJogada->ladoB = temp;
            }
            continue; 
        }

        int ladoEscolhido = -1;

        if (tabuleiro->tamanho == 0) {
             // Esta condição não deveria ocorrer se controlarTurnos já colocou a primeira peça.
             // Mas se ocorrer, a primeira peça é simplesmente adicionada.
            adicionarPecaNoCentro(tabuleiro, removerPedraDaMao(jogadorAtual, pedraJogada->ladoA, pedraJogada->ladoB));
            playedThisStone = 1;
            gameState->lastPlayedSide = 0; // Ou 1, arbitrário para a primeira peça
            gameState->lastPlayedByPlayerIndex = gameState->jogadorAtualIndex;
            gameState->lastPlayedWasGato = 0; // Primeira peça nunca é gato
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
        
        // Se a primeira peça foi jogada no if (tabuleiro->tamanho == 0)
        // não precisamos executar o código abaixo para adicionar a peça novamente.
        if (tabuleiro->tamanho > 0 || (tabuleiro->tamanho == 0 && playedThisStone == 0) ) {
            Pedra *pedraRemovidaDaMao = removerPedraDaMao(jogadorAtual, pedraJogada->ladoA, pedraJogada->ladoB);
            if (pedraRemovidaDaMao != NULL) {
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
                 if(querGirar){ // Desfaz o giro se a pedra não puder ser removida
                    int temp = pedraJogada->ladoA;
                    pedraJogada->ladoA = pedraJogada->ladoB;
                    pedraJogada->ladoB = temp;
                }
                screenClear();
                screenSetColor(RED, BLACK);
                printf("Erro ao remover a pedra da mao. Tente novamente.\n");
                screenSetColor(WHITE, BLACK);
                screenUpdate();
                printf("Pressione ENTER para continuar...\n");
                while (readch() != 10);
                continue; // Volta para selecionarPedraNaMao
            }
        }


        screenClear();
        screenSetColor(GREEN, BLACK);
        printf("%s jogou a pedra [%d|%d].\n", jogadorAtual->nome, pedraJogada->ladoA, pedraJogada->ladoB);
        if (gameState->lastPlayedWasGato) { // Usa o estado atualizado
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
        return 2; // Passou o turno (mão vazia)
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
        return 2; // Passou o turno (pedra única incompatível)
    }

    int finalActionChoice = -1;
    int actionExecutedSuccessfully = 0; // Flag para controlar se o menu principal deve ser re-exibido

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
                    // Não sai da função realizarJogada, o loop while(!actionExecutedSuccessfully) vai iterar
                    // e o menuEscolhaPrincipal será chamado novamente.
                    // O jogador DEVE jogar agora.
                    // Para forçar a jogada, podemos setar actionExecutedSuccessfully = 0 e continuar o loop
                    // ou chamar handlePlayStone diretamente, mas o menu é mais consistente.
                    // A lógica atual de re-chamar menuEscolhaPrincipal é ok, pois o estado do gato mudou.
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
            return 2; // Jogador passou
        } else if (finalActionChoice == OPTION_PLAY_STONE) {
            actionExecutedSuccessfully = 1; // Sai do loop e vai para handlePlayStone
        } else {
            // Opção inválida ou não tratada, apenas re-exibe o menu
            screenClear();
            screenSetColor(RED, BLACK);
            printf("Opcao invalida no menu. Tente novamente.\n");
            screenSetColor(WHITE, BLACK);
            screenUpdate();
            printf("Pressione ENTER para continuar...\n");
            while (readch() != 10);
        }
    } // Fim do while (!actionExecutedSuccessfully)

    return handlePlayStone(gameState, jogadorAtual); // Retorna 1 para vitória, 0 para jogada normal
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
        // Informações da primeira jogada (carroça)
        // O lado não importa muito para a primeira, mas vamos definir
        gameState.lastPlayedSide = 0; // Assumindo que a carroça é adicionada no centro/início
        // Quem jogou a "última" peça antes da primeira carroça? Ninguém.
        // Mas para a lógica de "gato", precisamos de um valor.
        // O jogador que jogou a carroça é (primeiroAJogarIndex - 1 + 4) % 4
        // E a próxima jogada é de primeiroAJogarIndex.
        // A lógica em controlarTurnos já retorna o *próximo* a jogar.
        // Então, quem jogou a carroça foi (gameState.jogadorAtualIndex - 1 + 4) % 4.
        gameState.lastPlayedByPlayerIndex = (gameState.jogadorAtualIndex - 1 + 4) % 4;
        gameState.lastPlayedWasGato = 0; // Primeira peça nunca é gato
    } else {
        printf("O jogo nao pode ser iniciado corretamente (sem carroca inicial).\n");
        printf("Pressione ENTER para finalizar...\n");
        while (readch() != 10);
        // Libera memória antes de sair
        Pedra *temp_free;
        while(gameState.dorme != NULL){ temp_free = gameState.dorme; gameState.dorme = gameState.dorme->next; free(temp_free); }
        if (gameState.tabuleiro.pecas != NULL) { free(gameState.tabuleiro.pecas); }
        for(int i = 0; i < 4; i++){ Pedra *current_hand = gameState.jogadores[i].mao; while(current_hand != NULL){ Pedra *temp_hand = current_hand; current_hand = current_hand->next; free(temp_hand); }}
        return;
    }

    printf("Iniciando o jogo. O proximo a jogar e o: %s\n\n", gameState.jogadores[gameState.jogadorAtualIndex].nome);
    printf("Pressione ENTER para iniciar o primeiro turno...\n");
    while(readch() != 10);

    while (!gameState.gameOver) {
        int jogadaResultado = realizarJogada(&gameState);

        if (jogadaResultado == 1) { 
            gameState.gameOver = 1;
            gameState.jogadorVencedorIndex = gameState.jogadorAtualIndex;
            gameState.consecutivePasses = 0; 
        } else if (jogadaResultado == 2) { 
            gameState.consecutivePasses++;
            gameState.jogadorAtualIndex = (gameState.jogadorAtualIndex + 1) % 4;
        } else { 
            gameState.consecutivePasses = 0; 
            gameState.jogadorAtualIndex = (gameState.jogadorAtualIndex + 1) % 4;
        }

        if (gameState.consecutivePasses >= 4) {
            gameState.gameOver = 1;
            gameState.jogadorVencedorIndex = -2; 
        }
    }

    screenClear();
    screenSetColor(CYAN, BLACK);
    printf("=====================\n");
    printf("===== FIM DE JOGO =====\n");
    printf("=====================\n\n");
    
    if (gameState.jogadorVencedorIndex == -2) { 
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
        printf("O jogo terminou por um motivo inesperado.\n");
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
        for(int i = 0; i < gameState.tabuleiro.tamanho; i++){
            if(gameState.tabuleiro.pecas[i] != NULL) { // Verifica antes de liberar
                free(gameState.tabuleiro.pecas[i]);
                gameState.tabuleiro.pecas[i] = NULL; // Boa prática
            }
        }
        free(gameState.tabuleiro.pecas);
        gameState.tabuleiro.pecas = NULL; // Boa prática
    }

    for(int i = 0; i < 4; i++){
        Pedra *current_hand = gameState.jogadores[i].mao;
        while(current_hand != NULL){
            Pedra *temp_hand = current_hand;
            current_hand = current_hand->next;
            free(temp_hand);
        }
        gameState.jogadores[i].mao = NULL; // Boa prática
    }
}
