#include "levenshtein.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int** init_dp(int m, int n){
    int** dp = malloc((m + 1) * sizeof(int*));
    for(int i = 0; i <= m; i++){
        dp[i] = malloc((n + 1) * sizeof(int));
    }
    return dp;
}

void free_dp(int**dp, int m){
    for(int i = 0; i <= m; i++){
        free(dp[i]);
    }
    free(dp);
}

int levenshtein(char* text, char* pattern, int k, int *pos, int* cont){
    int comp = 0;
    int n = strlen(text);
    int m = strlen(pattern);

    int **dp = init_dp(m, n);

    *cont = 0;

    for(int i = 0; i <= m; i++)
        dp[i][0] = i;
    for(int j = 0; j <= n; j++)
        dp[0][j] = 0;

    // Preenchimento
    for(int i = 1; i <= m; i++){
        for(int j = 1; j <= n; j++){
            int cost = (pattern[i - 1] == text[j - 1]) ? 0 : 1;
            int del = dp[i - 1][j] + 1;
            int ins = dp[i][j - 1] + 1;
            int sub = dp[i - 1][j - 1] + cost;
            int min = del < ins ? del : ins;
            dp[i][j] = (min < sub) ? min : sub;
        }
    }

    // Verifica onde o padrão "encaixa" no texto com até k erros
    for(int j = 0; j <= n; j++){
        comp++;
        if(j >= m && dp[m][j] <= k){
            pos[*cont] = j - m;
            (*cont)++;
        }
    }

    free_dp(dp, m);

    return comp;
}
