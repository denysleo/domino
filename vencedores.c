#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vencedores.h"

#define MAX_VENCEDORES 100
#define TAM_NOME 100

void mostrarUltimosVencedores() {
    FILE *arquivo = fopen("vencedores.txt", "r");
    if (arquivo == NULL) {
        printf("Ainda não há duplas vencedoras registradas.\n");
        return;
    }

    char vencedores[MAX_VENCEDORES][TAM_NOME];
    int count = 0;

    while (fgets(vencedores[count], TAM_NOME, arquivo) != NULL) {
        vencedores[count][strcspn(vencedores[count], "\n")] = '\0';
        count++;
        if (count >= MAX_VENCEDORES) break;
    }

    fclose(arquivo);

    printf("\n===== ÚLTIMAS 5 DUPLAS VENCEDORAS =====\n");
    int inicio = count >= 5 ? count - 5 : 0;
    for (int i = inicio; i < count; i++) {
        printf("%d. %s\n", i - inicio + 1, vencedores[i]);
    }
}

void registrarVencedores(const char *nome1, const char *nome2) {
    FILE *arquivo = fopen("vencedores.txt", "r");
    char vencedores[MAX_VENCEDORES][TAM_NOME];
    int count = 0; 

    if (arquivo != NULL) {

        while (fgets(vencedores[count], TAM_NOME, arquivo) != NULL && count < MAX_VENCEDORES) {
            vencedores[count][strcspn(vencedores[count], "\n")] = '\0';
            count++;
        }
        fclose(arquivo);
    }

    if (count == MAX_VENCEDORES) {
        for (int i = 1; i < MAX_VENCEDORES; i++) {
            strcpy(vencedores[i - 1], vencedores[i]);
        }
        count--;
    }


    snprintf(vencedores[count], TAM_NOME, "%s e %s", nome1, nome2);
    count++;

    arquivo = fopen("vencedores.txt", "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(arquivo, "%s\n", vencedores[i]);
    }

    fclose(arquivo);
}
