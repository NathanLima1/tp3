#include "alfabeto.h"

// Extração do proximo simbolo a ser codificado
void DefineAlfabeto(TipoAlfabeto Alfabeto, FILE* ArqAlf){
    // Os simbolos devem estar juntos em uma linha no arquivo

    char Simbolos[MaxAlfabeto +1];
    int i;
    char* Temp;
    for (i = 0; i <= MaxAlfabeto; i++) Alfabeto[i] = FALSE;
    fgets(Simbolos, MaxAlfabeto + 1, ArqAlf);
    Temp = strchr(Simbolos, '\n');
    if (Temp != NULL) *Temp = 0;
    for (i = 0; i < strlen(Simbolos); i++)
        Alfabeto[Simbolos[i] + 127] = TRUE;
    Alfabeto[0] = FALSE; /* caractere de código zero: separador */
}

void ExtraiProximaPalavra(TipoPalavra Result, int *Indice, char *Linha,
                          FILE *ArqTxt, TipoAlfabeto Alfabeto)
{
    short FimPalavra = FALSE, Aux = FALSE;
    Result[0] = '\0';

    if (*Indice >= strlen(Linha))
    {
        if (fgets(Linha, MaxAlfabeto + 1, ArqTxt))
        {
            /* Coloca um delimitador em Linha */
            sprintf(Linha + strlen(Linha), "%c", (char)0);
            *Indice = 0;
        }
        else { sprintf(Linha, "%c", (char)0); FimPalavra = TRUE; }
    }

    while (*Indice < strlen(Linha) && !FimPalavra)
    {
        if (!Alfabeto[Linha[*Indice] + 127])
        {
            if (Aux == FALSE)
            {
                Aux = TRUE;
                sprintf(Result + strlen(Result), "%c", Linha[*Indice]);
            }
            else
            {
                if (Linha[*Indice - 1] != (char)0) (*Indice)--;
                else { sprintf(Result + strlen(Result), "%c", Linha[*Indice - 1]); }
                FimPalavra = TRUE;
            }
        }
        else
        {
            sprintf(Result + strlen(Result), "%c", Linha[*Indice]);
            FimPalavra = TRUE;
        }

        (*Indice)++;
    }
}