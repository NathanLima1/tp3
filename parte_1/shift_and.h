#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void init_masks(int *masks, char *pattern, int m);
void shift_and(char *pattern, char *text, int n, int m, int k, FILE *fout);