#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_masks(int *masks, char *pattern, int m) {
    for (int i = 0; i < m; i++) {
        masks[(int)pattern[i]] |= (1 << i);
    }
}

int shift_and(char *pattern, char *text, int n, int m, int k, FILE *fout) {
    int *states = (int *)calloc((k + 1), sizeof(int));
    int *old_states = (int *)calloc((k + 1), sizeof(int));
    int masks[128] = {0};
    init_masks(masks, pattern, m);

    int comp = 0;
    fprintf(fout, "%s: ", pattern);

    for (int i = 0; i < n; i++) {
        memcpy(old_states, states, (k + 1) * sizeof(int));
        states[0] = ((states[0] << 1) | 1) & masks[(int)text[i]];

        for (int j = 1; j <= k; j++) {

            states[j] = 0;
            states[j] |= ((old_states[j] << 1) | 1) & masks[(int)text[i]]; // Troca, avança uma casa (Assumindo que casou) e verifica o caractere atual
            states[j] |= ((states[j - 1] << 1) | 1); // Deleção, avança uma casa sem verificar, permite que o padrão avance, mas o texto não
            states[j] |= old_states[j - 1]; // Inserção, apenas repassa o estado anterior para frente, permitindo que o texto avance, mas o padrão não
        }

        for (int j = 0; j <= k; j++) {
            comp++;
            if (states[j] & (1 << (m - 1))) {
                fprintf(fout, " %d", i); // Posição do último caractere do padrão
            }   
        }
    }

    fprintf(fout, "\n");

    free(states);
    return comp;
}