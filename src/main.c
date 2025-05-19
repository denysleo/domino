#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct pedra {
    int ladoA;
    int ladoB;
    struct pedra *next;
} Pedra;

typedef struct jogador {
    char nome[51];
    int pontuacao;
    Pedra *mao;
} Jogador;

Pedra *criarPedra(int ladoA, int ladoB);
Pedra *criarTodasAsPedras();
Pedra *removerPedraEspecificaDaMao(Jogador *jogador, int ladoA, int ladoB);

int main() {
    screenInit(1);
    keyboardInit();
    timerInit(50);

    screenClear();
    screenSetColor(CYAN, BLACK);
    printf("Dominó das ruas\n");
    screenUpdate();

    int ch = 0;
    while (ch != 10) {
        if (keyhit()) {
            ch = readch();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}


Pedra *criarPedra(int ladoA, int ladoB) {
    Pedra *nova = (Pedra *)malloc(sizeof(Pedra));
    if (nova == NULL) {
        return NULL;
    }
    nova->ladoA = ladoA;
    nova->ladoB = ladoB;
    nova->next = NULL;
    return nova;
}

Pedra *criarTodasAsPedras() {
    Pedra *head = NULL;
    Pedra *fim = NULL;

    for (int a = 0; a <= 6; a++) {
        for (int b = a; b <= 6; b++) {
            Pedra *nova = criarPedra(a, b);
            if (nova == NULL) {
                Pedra *atual = head;
                while (atual != NULL) {
                    Pedra *temp = atual;
                    atual = atual->next;
                    free(temp);
                }
                return NULL;
            }

            if (head == NULL) {
                head = nova;
                fim = nova;
            } else {
                fim->next = nova;
                fim = nova;
            }
        }
    }

    return head;
}

Pedra *removerPedraEspecificaDaMao(Jogador *jogador, int ladoA, int ladoB) {
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
