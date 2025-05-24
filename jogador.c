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

int calcularPontosDaMao(const Jogador *jogador) {
    int totalPontos = 0;
    Pedra *atual = jogador->mao;
    while (atual != NULL) {
        totalPontos += atual->ladoA + atual->ladoB;
        atual = atual->next;
    }
    return totalPontos;
}

int controlarTurnos(Jogador *jogadores, Tabuleiro *tabuleiro) {
    int jogadorInicial = -1;
    int maiorCarroca = -1;
    Pedra *pedraInicialObj = NULL;

    for (int valor = 6; valor >= 0; valor--) {
        for (int i = 0; i < 4; i++) {
            Pedra *m = jogadores[i].mao;
            while (m) {
                if (m->ladoA == valor && m->ladoB == valor) {
                    jogadorInicial = i;
                    maiorCarroca = valor;
                    pedraInicialObj = m; 
                    goto encontrou_carroca;
                }
                m = m->next;
            }
        }
    }

encontrou_carroca:
    if (jogadorInicial != -1 && pedraInicialObj != NULL) {
        Pedra *pedraParaTabuleiro = removerPedraDaMao(&jogadores[jogadorInicial], pedraInicialObj->ladoA, pedraInicialObj->ladoB);
        if (pedraParaTabuleiro) { 
             printf("Jogador que inicia: %s (com a pedra [%d|%d])\n\n", jogadores[jogadorInicial].nome, maiorCarroca, maiorCarroca);
             adicionarPecaNoCentro(tabuleiro, pedraParaTabuleiro);
             exibirTabuleiro(tabuleiro);
             return (jogadorInicial + 1) % 4; 
        } else {
            fprintf(stderr, "Erro: Carroca encontrada mas nao pode ser removida da mao do jogador %s.\n", jogadores[jogadorInicial].nome);
            return -1;
        }
    } else {
        printf("Nao foi possivel encontrar a pedra inicial (carroca) para iniciar o jogo.\n");
        printf("O jogo sera encerrado. Verifique a distribuicao de pecas.\n");
        return -1; 
    }
}

void readStringFromTerminal(char *buffer, int bufferSize) {
    int i = 0;
    int ch;
    screenSetColor(WHITE, BLACK); 
    screenUpdate(); 

    while (1) {
        if (keyhit()) {
            ch = readch();
            if (ch == 10) { 
                buffer[i] = '\0'; 
                break;
            } else if (ch == 127 || ch == 8) { 
                if (i > 0) {
                    i--;
                    printf("\b \b"); 
                    fflush(stdout); 
                }
            } else if (ch >= 32 && ch <= 126 && i < bufferSize - 1) {
                buffer[i++] = (char)ch;
                printf("%c", (char)ch); 
                fflush(stdout); 
            }
        }
    }
}

void distribuirPecas(Jogador *jogadores, Pedra *todas, Pedra **dorme) {
    Pedra *ptr = todas;
    for (int j = 0; j < 4; j++) {
        jogadores[j].mao = NULL;
        jogadores[j].pontuacao = 0;
        printf("Digite o nome do Jogador %d: ", j + 1);
        readStringFromTerminal(jogadores[j].nome, sizeof(jogadores[j].nome));
        printf("\n"); 
    }

    for (int k = 0; k < 6; k++) { 
        for (int j = 0; j < 4; j++) {
            if (!ptr) {
                fprintf(stderr, "Erro: Faltaram pecas para distribuir!\n");
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

int selecionarPedraNaMao(Jogador *jogador, const GameState *gameState, Pedra **pedraSelecionadaPtr) {
    *pedraSelecionadaPtr = NULL; 

    if (jogador->mao == NULL) {
        printf("Sua mao esta vazia. Nao ha pedras para jogar.\n");
        printf("Pressione ENTER para continuar...\n");
        int auto_tecla = 0;
        // Aqui, como não há navegação por setas, podemos usar readch() diretamente
        // ou a função auxiliar lerTeclaProcessadaComDebug() se quisermos consistência.
        // Por simplicidade, se a cli-lib não processa ESC corretamente, o usuário teria que apertar enter.
        // Para permitir ESC aqui também, precisaríamos da lógica de lerTeclaProcessadaComDebug.
        // Vamos assumir por agora que o usuário pressionará ENTER ou o ESC será um 27 direto.
        while(auto_tecla != 10 && auto_tecla != KEY_ESC) { 
            if(keyhit()) {
                 auto_tecla = readch(); // Usando readch() diretamente
                 // Se readch() retorna 27 para setas, este loop pode sair com setas.
                 // A função lerTeclaProcessadaComDebug() de jogo.c seria mais robusta aqui.
            }
        } 
        if(auto_tecla == KEY_ESC) return STATUS_GAME_QUIT;
        return STATUS_GAME_CONTINUE; 
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
    int tecla_processada = 0; 
    int mudou = 1;

    while (tecla_processada != 10 ) { // Loop até ENTER ou ESC (tratado abaixo)
        if (mudou) {
            screenClear();
            screenSetColor(WHITE, BLACK);
            printf("Turno de %s (ESC para Sair do Jogo)\n", jogador->nome);
            printf("Tabuleiro atual: ");
            exibirTabuleiro(&gameState->tabuleiro);
            printf("\n");
            printf("Mao de %s:\n", jogador->nome);
            printf("Use as setas ESQUERDA/DIREITA para selecionar. ENTER para confirmar. ESC para Sair.\n\n");
            for (int i = 0; i < numPedras; i++) {
                if (i == selectedIndex) screenSetColor(YELLOW, BLACK);
                else screenSetColor(WHITE, BLACK);
                printf("[%d|%d] ", arrayPedras[i]->ladoA, arrayPedras[i]->ladoB);
            }
            printf("\n\n");
            screenUpdate();
            mudou = 0;
        }

        if (keyhit()) {
            int raw_tecla = readch();
            tecla_processada = raw_tecla; // Valor padrão

            if (raw_tecla == KEY_ESC) {
                int c2 = -1, c3 = -1;
                if (keyhit()) {
                    c2 = readch();
                    if (c2 == '[') {
                        if (keyhit()) {
                            c3 = readch();
                            if (c3 == 'A') { tecla_processada = 65; } // Seta Cima
                            else if (c3 == 'B') { tecla_processada = 66; } // Seta Baixo
                            else if (c3 == 'C') { tecla_processada = 67; } // Seta Direita
                            else if (c3 == 'D') { tecla_processada = 68; } // Seta Esquerda
                            else { tecla_processada = KEY_ESC; } // Sequência não reconhecida
                        } else { tecla_processada = KEY_ESC; } // ESC [ incompleto
                    } else { tecla_processada = KEY_ESC; } // ESC seguido por não '['
                } else { tecla_processada = KEY_ESC; } // ESC isolado
            }
            
            if (tecla_processada == KEY_ESC) {
                 free(arrayPedras);
                 return STATUS_GAME_QUIT;
            }
            
            mudou = 0;
            if (tecla_processada == 68 && selectedIndex > 0) { // Seta Esquerda
                selectedIndex--;
                mudou = 1;
            } else if (tecla_processada == 67 && selectedIndex < numPedras - 1) { // Seta Direita
                selectedIndex++;
                mudou = 1;
            } else if (tecla_processada == 65 || tecla_processada == 66) {
                // Setas Cima/Baixo são reconhecidas mas não fazem nada neste menu
                // O importante é que não sejam tratadas como ESC
                mudou = 1; // Força redesenho para limpar qualquer artefato de debug se houver
            }
        }
    }

    // Se saiu do loop, foi por tecla_processada == 10 (ENTER)
    *pedraSelecionadaPtr = arrayPedras[selectedIndex];
    free(arrayPedras);
    return STATUS_GAME_CONTINUE; 
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
    if (tabuleiro->tamanho == 0) return 1; 

    Pedra *current = jogador->mao;
    while (current != NULL) {
        if (isCompatible(current, tabuleiro)) {
            return 1;
        }
        if (current->ladoA != current->ladoB) {
            Pedra pedraGirada;
            pedraGirada.ladoA = current->ladoB;
            pedraGirada.ladoB = current->ladoA;
            if (isCompatible(&pedraGirada, tabuleiro)) {
                return 1;
            }
        }
        current = current->next;
    }
    return 0;
}
