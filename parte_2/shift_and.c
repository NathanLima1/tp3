#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int k = 2;
    int *states = (int *)malloc(k * sizeof(int));

    char *text = "texto para testar o tes dte\0";
    char *padrao = "teste\0";

    int n = strlen(text);
    int m = strlen(padrao);

    int *masks = (int*)calloc(26, sizeof(int));

    int j = 0;
    for (int i = 0; i < m; i++) {
        j = padrao[i] - 97;
        if (masks[j] != 0) continue; // Já processou essa letra
        for (int k = 0; k < m; k++) {
            masks[j] = (masks[j] << 1) + (j + 97 == padrao[k]); // Desloca 1 para a esquerda e adiciona 1 se for igual
        }
    }

    for (int i = 0; i < 26; i++) {
        printf("%c ", i + 97);
    }
    printf("\n");
    for (int i = 0; i < 26; i++) {
        printf("%d ", masks[i]);
    }
    printf("\n");
    free(states);
    return 0;
}