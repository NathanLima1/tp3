#include "descompressao.h"

// codigo para fazer descompressão
void Descompressao(FILE *ArqComprimido, FILE *ArqTxt, FILE *ArqAlf)
{
    TipoAlfabeto Alfabeto; int Ind, MaxCompCod;
    TipoVetorPalavra Vocabulario; TipoVetoresBO VetoresBaseOffset;
    TipoPalavra PalavraAnt;

    DefineAlfabeto(Alfabeto, ArqAlf); /* Lê alfabeto definido em arquivo */
    MaxCompCod = LeVetores(ArqComprimido, VetoresBaseOffset);
    LeVocabulario(ArqComprimido, Vocabulario);

    Ind = Decodifica(VetoresBaseOffset, ArqComprimido, MaxCompCod);
    fputs(Vocabulario[Ind], ArqTxt);

    while (!feof(ArqComprimido))
    {
        Ind = Decodifica(VetoresBaseOffset, ArqComprimido, MaxCompCod);
        if (Ind > 0)
        {
            if (Alfabeto[Vocabulario[Ind][0] + 127] && PalavraAnt[0] != '\n')
                putc(' ', ArqTxt);
            strcpy(PalavraAnt, Vocabulario[Ind]);
            fputs(Vocabulario[Ind], ArqTxt);
        }
    }
}

// funções auxiliares da descompressão
int LeVetores(FILE *ArqComprimido, TipoBaseOffset *VetoresBaseOffset)
{
    int MaxCompCod, i;
    MaxCompCod = LeNumInt(ArqComprimido);

    for (i = 1; i <= MaxCompCod; i++)
    {
        VetoresBaseOffset[i].Base = LeNumInt(ArqComprimido);
        VetoresBaseOffset[i].Offset = LeNumInt(ArqComprimido);
    }

    return MaxCompCod;
}

// Em descompressao.c

int LeVocabulario(FILE *ArqComprimido, TipoVetorPalavra Vocabulario)
{
    int NumNodosFolhas, i;
    TipoPalavra Palavra;
    char Ch;
    size_t len;

    NumNodosFolhas = LeNumInt(ArqComprimido);
    // --- printf de depuração ---
    printf("\n--- Lendo Vocabulário ---\n");
    printf("Cabeçalho diz que existem %d palavras (nós folhas) no dicionário.\n", NumNodosFolhas);
    // --- Fim do printf ---

    for (i = 1; i <= NumNodosFolhas; i++) {
        // --- printf de depuração ---
        printf("Tentando ler palavra #%d...\n", i);
        // --- Fim do printf ---

        Palavra[0] = '\0';
        len = 0;

        do {
            if (fread(&Ch, sizeof(unsigned char), 1, ArqComprimido) != 1) {
                // Se a leitura falhar, vamos ver por quê
                if (feof(ArqComprimido)) {
                    fprintf(stderr, "Erro: Fim de arquivo inesperado no meio da leitura da palavra #%d.\n", i);
                } else {
                    // Este é o erro que você está vendo
                    fprintf(stderr, "Erro de leitura do stream ao tentar ler a palavra #%d.\n", i);
                }
                // Vamos parar aqui para não continuar com dados corrompidos
                exit(EXIT_FAILURE);
            }

            if (Ch != (char)0) {
                if (len < MAX_WORD - 1) {
                    Palavra[len++] = Ch;
                }
            }
        } while (Ch != (char)0);

        Palavra[len] = '\0';
        
        // --- printf de depuração ---
        printf("Palavra #%d lida com sucesso: [%s]\n", i, Palavra);
        // --- Fim do printf ---

        strncpy(Vocabulario[i], Palavra, MAX_WORD);
        Vocabulario[i][MAX_WORD - 1] = '\0';
    }

    printf("--- Leitura do Vocabulário concluída com sucesso ---\n\n");
    return NumNodosFolhas;
}
