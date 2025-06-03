#include "levenshtein.h"

int get_min2(int a, int b){
    return a < b ? a : b;
}

int get_min3(int a, int b, int c){
    int min2 = get_min2(b, c);
    return a < min2 ? a : min2;
}

void print_matrix(int** dp, int n, int m){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            printf("%d ", dp[i][j]);
        }
        printf("\n");
    }
}

int** init_dp(int n, int m){
    int** dp = malloc(n * sizeof(int*));
    for(int i = 0; i < n; i++){
        dp[i] = malloc(m*sizeof(int));
    }
    return dp;
}

void free_dp(int**dp, int n){
    for(int i = 0; i < n; i++){
        free(dp[i]);
    }
    free(dp);
}

int levenshtein(int** dp, char* s1, char* s2, int n, int m){
    // preenche a primeira linha e a primeira coluna com os indices das letras das palavras
    for(int i = 0; i < n+1; i++){
        dp[i][0] = i;
    }
    for(int j = 0; j < m+1; j++){
        dp[0][j] = j;
    }

    // com base nas comparações anteriores decide 
    for(int i = 1; i < n+1; i++){
        for(int j = 1; j < m+1; j++){
            // se o caractere atual é igual, o custo é 0
            int cost = (s1[i-1] == s2[j-1]) ? 0 : 1;
            dp[i][j] = get_min3(dp[i-1][j]+cost, dp[i][j-1]+cost, dp[i-1][j-1] + cost);
        }
    }
    print_matrix(dp, n+1, m+1);

    return dp[n][m];
}
