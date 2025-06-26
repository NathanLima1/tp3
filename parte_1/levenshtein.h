#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_matrix(int** dp, int n, int m);

int** init_dp(int m, int n);

void free_dp(int**dp, int n);

int levenshtein(char* text, char* pattern, int k, int *pos, int* cont);
