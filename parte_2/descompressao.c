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

    NumNodosFolhas = LeNumInt(ArqComprimido);
    for (i = 1; i <= NumNodosFolhas; i++)
    {
        *Palavra = '\0';
        do
        {
            fread(&Ch, sizeof(unsigned char), 1, ArqComprimido);
            if (Ch != (char)0)  /* Palavras separadas pelo caractere 0 */
                sprintf(Palavra + strlen(Palavra), "%c", Ch);
        } while (Ch != (char)0);

        strcpy(Vocabulario[i], Palavra);
    }

    return NumNodosFolhas;
}