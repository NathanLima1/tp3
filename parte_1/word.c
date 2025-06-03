#include "word.h"

Padrao* init_padroes(int n) {
    Padrao* p = malloc(sizeof(Padrao) * (n + 1));
    for (int i = 0; i <= n; i++) {
        p[i].capacity = 0;
        p[i].len = 3;
        p[i].positions = malloc(sizeof(int) * p[i].len);
        for (int j = 0; j < p[i].len; j++) {
            p[i].positions[j] = 0;
        }
    }
    return p;
}

void free_padroes(Padrao* p, int n) {
    if (p == NULL) return;

    for (int i = 0; i <= n; i++) {
        free(p[i].positions);
    }
    free(p);
}

void insert_position(Padrao* p, int pos){
    if(p->capacity >= p->len){
        p->len *= 2;
        p->positions = realloc(p->positions, p->len*sizeof(int));
    }
    p->positions[p->capacity] = pos;
    p->capacity++;
}