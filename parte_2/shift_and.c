#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int k = 2;
    int *states = (int *)malloc(k * sizeof(int));

    char *text = "texto para testar o teste\0";
    char *padrao = "teste\0";

    int n = strlen(text);
    int m = strlen(padrao);

    int *masks = (int*)malloc(26 * sizeof(int));

    int j = 0;
    for (int i = 0; i < 26; i++) {
        masks[i] = 0;
        for (int k = 0; k < m; k++) {
            masks[i] = (masks[i] << 1) + (i + 97 == padrao[k]); // Desloca 1 para a esquerda e adiciona 1 se for igual
        }
        j++;
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