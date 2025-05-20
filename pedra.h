#ifndef PEDRA_H
#define PEDRA_H

typedef struct pedra {
    int ladoA;
    int ladoB;
    struct pedra *next;
} Pedra;

Pedra *criarPedra(int ladoA, int ladoB);
Pedra *criarTodasAsPedras();

#endif
