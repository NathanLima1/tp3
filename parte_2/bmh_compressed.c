#include "bmh_compressed.h"

void BuscaMultiplosPadroes(FILE *ArqComprimido, FILE *ArqAlf,
                            FILE *ArqPadroes, FILE *ArqSaida) {
    TipoAlfabeto Alfabeto;
    TipoVetorPalavra Vocabulario;
    TipoVetoresBO VetoresBaseOffset;
    TipoTexto T;
    TipoPadrao Padrao;
    char p[256];
    int MaxCompCod, NumNodosFolhas;
    int c, Ord, Ind, n = 1;

    DefineAlfabeto(Alfabeto, ArqAlf);
    MaxCompCod = LeVetores(ArqComprimido, VetoresBaseOffset);
    NumNodosFolhas = LeVocabulario(ArqComprimido, Vocabulario);

    // Lê o conteúdo comprimido
    while (fread(&T[n], sizeof(char), 1, ArqComprimido)) n++;

    rewind(ArqPadroes);
    while (fgets(p, 256, ArqPadroes)) {
        p[strcspn(p, "\n")] = '\0'; // remove '\n'

        // Busca palavra no vocabulário
        Ord = -1;
        for (Ind = 1; Ind <= NumNodosFolhas; Ind++) {
            if (!strcmp(Vocabulario[Ind], p)) {
                Ord = Ind;
                break;
            }
        }

        if (Ord == -1) continue; // não está no vocabulário

        int Codigo = Codifica(VetoresBaseOffset, Ord, &c, MaxCompCod);
        Atribui(Padrao, Codigo, c);

        // Busca com BMH
        int ocorrencias[1000];
        int qtd = BMH_busca_ocorrencias(T, n, Padrao, c, ocorrencias);

        fprintf(ArqSaida, "%s", p);
        for (int i = 0; i < qtd; i++) {
            fprintf(ArqSaida, " %d", ocorrencias[i]);
        }
        fprintf(ArqSaida, "\n");
    }
}

int BMH_busca_ocorrencias(unsigned char *T, int n,
                          unsigned char *P, int m,
                          int *ocorrencias) {
    int i, j, d[MAX_TEXTO], qtd = 0;

    for (j = 0; j < 256; j++) d[j] = m;
    for (j = 0; j < m - 1; j++) d[P[j]] = m - j - 1;

    i = m - 1;
    while (i < n) {
        int k = 0;
        while (k < m && P[m - 1 - k] == T[i - k]) k++;

        if (k == m) {
            ocorrencias[qtd++] = i - m + 1;
        }
        i += d[T[i]];
    }

    return qtd;
}
