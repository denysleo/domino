#include "pedra.h"
#include <stdlib.h>

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
