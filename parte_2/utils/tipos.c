#include "tipos.h"

static unsigned fHash(TipoChave Ch, const TipoPesos p)
{
    unsigned h = 0;
    for (int i = 0; Ch[i] != '\0'; ++i)
        h = (h + (unsigned)Ch[i] * p[i]) % M;
    return h + 1;                /* posições válidas: 1..M */
}

void GeraPesos(TipoPesos p)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < MAX_WORD; ++i)
        p[i] = (rand() << 16) ^ rand() ^ (rand() << 8);
}

void Inicializa(TipoDicionario D)
{
    for (int i = 0; i <= M; ++i) {
        D[i].Chave[0] = '\0';
        D[i].Freq     = 0;
        D[i].Ordem    = 0;
    }
}

int Pesquisa(TipoChave Ch, TipoPesos p, TipoDicionario D)
{
    unsigned i = fHash(Ch, p); 
    unsigned j = 1;
    while (strcmp(D[i].Chave, VAZIO) &&
           strcmp(Ch, D[i].Chave) && j <= M) {
        i = i % M + 1;
        ++j;
    }
    return (strcmp(Ch, D[i].Chave) == 0) ? (int)i : M;
}

void Insere(TipoItem x, TipoPesos p, TipoDicionario D)
{
    unsigned i = fHash(x.Chave, p);
    while (strcmp(D[i].Chave, VAZIO))
        i = i % M + 1;
    D[i] = x;
}

char *Trim(char *s)
{
    /* remove espaços à esquerda e à direita */
    char *end;
    while (isspace((unsigned char)*s)) ++s;
    if (*s == 0) return s;
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) --end;
    *(end + 1) = '\0';
    return s;
}

static int Particao(TipoDicionario A, int esq, int dir)
{
    int i = esq, j = dir;
    TipoItem x = A[(esq + dir) / 2], w;
    while (i <= j) {
        while (A[i].Freq > x.Freq) ++i;    
        while (A[j].Freq < x.Freq) --j;
        if (i <= j) {
            w = A[i]; A[i] = A[j]; A[j] = w;
            ++i; --j;
        }
    }
    return i;
}

static void QuickRec(TipoDicionario A, int esq, int dir)
{
    int p = Particao(A, esq, dir);
    if (esq < p - 1) QuickRec(A, esq, p - 1);
    if (p   < dir)   QuickRec(A, p,   dir);
}

void QuickSort(TipoDicionario A, int *n)
{
    if (*n > 1) QuickRec(A, 1, *n);
}

static void PreBmBc(const unsigned char *pat, int m, int bmBc[256])
{
    for (int i = 0; i < 256; ++i) bmBc[i] = m;
    for (int i = 0; i < m - 1; ++i) bmBc[pat[i]] = m - 1 - i;
}

void BMH(unsigned char *T, int n,
         unsigned char *P, int m)
{
    int bmBc[256];
    PreBmBc(P, m, bmBc);

    int j = 0;
    while (j <= n - m) {
        if (P[m - 1] == T[j + m - 1] &&
            memcmp(P, T + j, m - 1) == 0)
            printf("Padrão encontrado na posição %d\n", j);
        j += bmBc[T[j + m - 1]];
    }
}
