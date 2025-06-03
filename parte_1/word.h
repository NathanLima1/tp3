#include <stdio.h>
#include <stdlib.h>

typedef struct padrao{
    char* word;
    int* positions;
    int len;
    int capacity;
}Padrao;

Padrao* init_padroes(int n);

void free_padroes(Padrao* p, int n);

void insert_position(Padrao* p, int pos);