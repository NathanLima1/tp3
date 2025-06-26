#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_masks(int *masks, char *pattern, int m) {
    for (int i = 0; i < m; i++) {
        masks[(int)pattern[i]] |= (1 << i);
    }
}

void shift_and(char *pattern, char *text, int n, int m, int k, FILE *fout) {
    int *states = (int *)calloc((k + 1), sizeof(int));

    int masks[128] = {0};
    init_masks(masks, pattern, m);

    fprintf(fout, "%s: ", pattern);
    for (int i = 0; i < n; i++) {
        int prev_state = states[0];
        states[0] = ((states[0] << 1) | 1) & masks[(int)text[i]];

        for (int j = 1; j <= k; j++) {
            int tmp = states[j];
            states[j] = ((states[j] << 1) | 1) & masks[(int)text[i]];

            states[j] |= (prev_state << 1); // Deleção, avança uma casa na verificação, pula a verificação antiga e deixa para o próximo and
            states[j] |= prev_state; // Inserção, preserva o estado anterior, ignorando a verificação atual
            states[j] |= (states[j - 1] << 1); // Troca, avança no estado atual para ignorar o próximo carácter (será lido futuramente)

            prev_state = tmp;       
        }

        for (int j = 0; j <= k; j++) {
            if (states[j] & (1 << (m - 1))) {
                fprintf(fout, " %d", i + 1); // Posição do último caractere do padrão
            }   
        }
    }

    fprintf(fout, "\n");

    free(states);
}