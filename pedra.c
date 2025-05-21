#include "pedra.h"
#include <stdlib.h>
#include <time.h>

#define TOTAL_PECAS 28

Pedra *criarPedra(int ladoA, int ladoB) {
    Pedra *nova = (Pedra *)malloc(sizeof(Pedra));
    if (!nova) return NULL;
    nova->ladoA = ladoA;
    nova->ladoB = ladoB;
    nova->next = NULL;
    return nova;
}

Pedra *criarTodasAsPedras() {
    Pedra *head = NULL, *fim = NULL;
    for (int a = 0; a <= 6; a++) {
        for (int b = a; b <= 6; b++) {
            Pedra *nova = criarPedra(a, b);
            if (!nova) return NULL;
            if (!head) {
                head = fim = nova;
            } else {
                fim->next = nova;
                fim = nova;
            }
        }
    }
    return head;
}

void embaralharListaEncadeada(Pedra **headRef) {
    if (!headRef || !(*headRef)) return;

    Pedra *array[TOTAL_PECAS];
    int i = 0;
    Pedra *atual = *headRef;

    // Copia os ponteiros para o array
    while (atual != NULL && i < TOTAL_PECAS) {
        array[i++] = atual;
        atual = atual->next;
    }

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Fisher-Yates shuffle
    for (int j = i - 1; j > 0; j--) {
        int k = rand() % (j + 1);
        Pedra *tmp = array[j];
        array[j] = array[k];
        array[k] = tmp;
    }

    // Reconecta os elementos da lista com a nova ordem
    for (int j = 0; j < i - 1; j++) {
        array[j]->next = array[j + 1];
    }
    array[i - 1]->next = NULL;

    // Atualiza a cabeça da lista
    *headRef = array[0];
}

