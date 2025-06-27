#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bmh(char *pattern, char *text, int m, int n, FILE *fp) {
    int skip_table[128];
    int found = 0;

    for (int i = 0; i < 128; i++) {
        skip_table[i] = m;
    }

    for (int i = 0; i < m - 1; i++) {
        skip_table[(unsigned char)pattern[i]] = m - 1 - i;
    }

    int pos = 0;

    printf("%s: ", pattern);

    while (pos <= n - m) {
        int pattern_pos = m - 1;

        while (pattern_pos >= 0 && text[pos + pattern_pos] == pattern[pattern_pos]) {
            pattern_pos--;
        }

        if (pattern_pos < 0) {
            printf("%d ", pos);
            found = 1;
            pos++;
        } else {
            int c = text[pos + m - 1];
            pos += skip_table[c];
        }
    }

    if (!found) {
        printf("não encontrado");
    }
    printf("\n");
}
