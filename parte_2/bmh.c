#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bmh(char *s1, char *s2, int m, int n) {
    int skip_table[128];
    for(int i = 0; i < 128; i++) {
        skip_table[i] = m;
    }

    for(int i = 0; i < m - 1; i++) {
        skip_table[s1[i]] = m - i - 1;
    }

    int last_pos = n - m;
    int pos = 0;
    while(pos <= last_pos) {

        int pattern_pos = m - 1;
        while (pattern_pos >= 0 && s2[pos + pattern_pos] == s1[pattern_pos]) {
            pattern_pos--;
        }

        if(pattern_pos == -1) {
            printf("Casou %d\n", pos);
            pos++;
        } else {
            // Se não casou, pula até o próximo casar
            pos += skip_table[s2[pos + m - 1]]; // Último carácter sendo avaliado
        }
    }
}