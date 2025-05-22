#include "pedra.h"
#include <stdlib.h>
#include <time.h>

#define TOTAL_PECAS 28

Pedra *criarPedra(int ladoA, int ladoB) {
    Pedra *nova = (Pedra *)malloc(sizeof(Pedra));
    if (!nova) return NULL;
    nova->ladoA = ladoA;
    nova->ladoB = ladoB;
    nova->dir = NULL;
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
                fim->dir = nova;
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

    while (atual != NULL && i < TOTAL_PECAS) {
        array[i++] = atual;
        atual = atual->dir;
    }

    srand(time(NULL));

    for (int j = i - 1; j > 0; j--) {
        int k = rand() % (j + 1);
        Pedra *tmp = array[j];
        array[j] = array[k];
        array[k] = tmp;
    }

    for (int j = 0; j < i - 1; j++) {
        array[j]->dir = array[j + 1];
    }
    array[i - 1]->dir = NULL;

    *headRef = array[0];
}
