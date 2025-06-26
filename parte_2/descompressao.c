#include "descompressao.h"

// codigo para fazer descompressão
void Descompressao(FILE *ArqComprimido, FILE *ArqTxt, FILE *ArqAlf)
{
    TipoAlfabeto Alfabeto; int Ind, MaxCompCod;
    TipoVetorPalavra Vocabulario; TipoVetoresBO VetoresBaseOffset;
    TipoPalavra PalavraAnt;

    DefineAlfabeto(Alfabeto, ArqAlf); /* Lê alfabeto definido em arquivo */
    MaxCompCod = LeVetores(ArqComprimido, VetoresBaseOffset);
    LeVocabulario(Vocabulario, ArqComprimido);

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

int LeVocabulario(FILE *ArqComprimido, TipoVetorPalavra Vocabulario)
{
    int NumNodosFolhas, i;
    TipoPalavra Palavra;
    char Ch;
    size_t len;

    NumNodosFolhas = LeNumInt(ArqComprimido);

    for (i = 1; i <= NumNodosFolhas; i++) {
        Palavra[0] = '\0';
        len = 0;

        do {
            if (fread(&Ch, sizeof(unsigned char), 1, ArqComprimido) != 1) {
                fprintf(stderr, "Erro ao ler caractere do arquivo comprimido.\n");
                exit(EXIT_FAILURE);
            }

            if (Ch != (char)0) {
                if (len < MAX_WORD - 1) {
                    Palavra[len++] = Ch;
                } else {
                    fprintf(stderr, "Erro: palavra excede tamanho máximo (%d).\n", MAX_WORD);
                    exit(EXIT_FAILURE);
                }
            }
        } while (Ch != (char)0);

        Palavra[len] = '\0';

        // Cópia segura para o vocabulário
        strncpy(Vocabulario[i], Palavra, MAX_WORD);
        Vocabulario[i][MAX_WORD - 1] = '\0'; // Garantia de terminação
    }

    return NumNodosFolhas;
}
