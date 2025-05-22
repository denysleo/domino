#ifndef PEDRA_H
#define PEDRA_H

typedef struct pedra {
    int ladoA;
    int ladoB;
    struct pedra *esq;
    struct pedra *dir;
} Pedra;

Pedra *criarPedra(int ladoA, int ladoB);
Pedra *criarTodasAsPedras();
void embaralharListaEncadeada(Pedra **headRef);

#endif
