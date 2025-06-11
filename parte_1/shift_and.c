#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shift_and(char *s1, char *s2, int n, int m, int k) {
    int *states = (int *)calloc((k + 1), sizeof(int));

    int masks[128] = {0};
    for (int i = 0; i < m; i++) {
        masks[(int)s2[i]] |= (1 << i);
    }

    for (int i = 0; i < n; i++) {
        int prev_state = states[0];
        states[0] = ((states[0] << 1) | 1) & masks[(int)s1[i]];

        for (int j = 1; j <= k; j++) {
            int tmp = states[j];
            states[j] = ((states[j] << 1) | 1) & masks[(int)s1[i]];

            states[j] |= (prev_state << 1); // Inserção, avança uma casa na verificação
            states[j] |= prev_state; // Deleção, apenas ignora o atual, preservando o anterior (Sem as máscaras)
            states[j] |= states[j - 1]; // Troca, inora o atual e preserva o anterior (Já com as máscaras, exemplo para "testem" e "testam")

            prev_state = tmp;
        }

        for (int j = 0; j <= k; j++) {
            if (states[j] & (1 << (m - 1))) {
                printf("Casamento %d em: %d\n", j, i - m + 1);
            }   
        }
    }

    free(states);
    return 0;
}