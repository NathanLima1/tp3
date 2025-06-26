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
    int i = 0;
    Result[0] = '\0'; // Limpa a palavra resultado

    // Se a linha atual já foi toda lida, pega a próxima linha do arquivo
    if (Linha[*Indice] == '\0') {
        if (fgets(Linha, MaxAlfabeto + 2, ArqTxt) == NULL) {
            // Se não há mais linhas, retorna (Result continuará vazio)
            return;
        }
        *Indice = 0; // Reseta o índice para o início da nova linha
    }

    // Pula quaisquer separadores no início
    // (mas apenas se não for a primeira palavra da linha)
    // Esta parte é complexa, vamos simplificar o tratamento de espaço:
    // O espaço será um token.

    char char_atual = Linha[*Indice];

    // Caso 1: O caractere atual é um separador (não está no alfabeto)
    if (char_atual != '\0' && !Alfabeto[char_atual + 127]) {
        Result[0] = char_atual;
        Result[1] = '\0';
        (*Indice)++; // Avança um caractere
        printf("Palavra (separador) extraida: [%s]\n", Result);
        return;
    }

    // Caso 2: O caractere atual pertence ao alfabeto
    // Lê todos os caracteres sequenciais que pertencem ao alfabeto
    while ( (char_atual = Linha[*Indice]) != '\0' && Alfabeto[char_atual + 127] ) {
        if (i < MAX_WORD - 1) {
            Result[i++] = char_atual;
        }
        (*Indice)++;
    }
    Result[i] = '\0'; // Finaliza a string da palavra

    printf("Palavra (texto) extraida: [%s]\n", Result);
}
