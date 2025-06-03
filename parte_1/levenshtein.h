#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_min2(int a, int b);

int get_min3(int a, int b, int c);

void print_matrix(int** dp, int n, int m);

int** init_dp(int n, int m);

void free_dp(int**dp, int n);

int levenshtein(int** dp, char* s1, char* s2, int n, int m);
