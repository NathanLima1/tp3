#include "compressao.h"

// Proxima etapa da compressão

void PrimeiraEtapa(FILE *ArqTxt, TipoAlfabeto Alfabeto, int *Indice,
                   TipoPalavra Palavra, char *Linha,
                   TipoDicionario Vocabulario, TipoPesos p)
{
    TipoItem Elemento;
    int i;

    do {
        ExtraiProximaPalavra(Palavra, Indice, Linha, ArqTxt, Alfabeto);
        strncpy(Elemento.Chave, Palavra, MAX_WORD - 1);
        Elemento.Chave[MAX_WORD - 1] = '\0';
        Elemento.Freq = 1;

        if (*Palavra != '\0')
        {
            i = Pesquisa(Elemento.Chave, p, Vocabulario);
            if (i < M)
                Vocabulario[i].Freq++;
            else
                Insere(Elemento, p, Vocabulario);

            do {
                ExtraiProximaPalavra(Palavra, Indice, Linha,
                                     ArqTxt, Alfabeto);
                memcpy(Elemento.Chave, Palavra, sizeof(TipoChave));
                /* O primeiro espaço depois da palavra nao é codificado */
                if (strcmp(Trim(Palavra), "") &&
                    *(Trim(Palavra)) != (char)0)
                {
                    i = Pesquisa(Elemento.Chave, p, Vocabulario);
                    if (i < M)
                        Vocabulario[i].Freq++;
                    else
                        Insere(Elemento, p, Vocabulario);
                }
            } while (strcmp(Palavra, ""));
        }
    } while (Palavra[0] != '\0');
}

// Segunda etapa da compressão
int SegundaEtapa (TipoDicionario Vocabulario,
                  TipoVetoresBO VetoresBaseOffset,
                  TipoPesos p,
                  FILE *ArqComprimido)
{
    int Result, i, j, NumNodosFolhas;
    char Ch;

    // Ordena o vocabulário por frequência para preparar para a codificação
    NumNodosFolhas = OrdenaPorFrequencia(Vocabulario);
    
    // Calcula os comprimentos dos códigos de Huffman
    CalculaCompCodigo(Vocabulario, NumNodosFolhas);
    
    // Constrói e grava os vetores Base e Offset no arquivo
    Result = ConstroiVetores(VetoresBaseOffset, Vocabulario,
                              NumNodosFolhas, ArqComprimido);

    // Grava o número de palavras (nós folhas) no arquivo
    GravaNumInt(ArqComprimido, NumNodosFolhas);

    // Grava o Vocabulario (a lista de palavras) no arquivo
    for (i = 1; i <= NumNodosFolhas; i++)
    {
        j = 0;
        // Escreve cada caractere da palavra
        while (Vocabulario[i].Chave[j] != '\0')
        {
            fwrite(&Vocabulario[i].Chave[j], sizeof(char), 1, ArqComprimido);
            j++;
        }
        // Escreve o caractere nulo para marcar o fim da palavra
        Ch = '\0';
        fwrite(&Ch, sizeof(char), 1, ArqComprimido);
    }
    
    // A complexa e perigosa parte de "releitura" foi REMOVIDA.
    // A função agora simplesmente escreve o cabeçalho e retorna.
    // A reconstrução da tabela de hash a partir do arquivo
    // é responsabilidade do módulo de descompressão/busca.

    return Result;
}

void TerceiraEtapa(FILE *ArqTxt, TipoAlfabeto Alfabeto, int *Indice,
                   TipoPalavra Palavra, char *Linha,
                   TipoDicionario Vocabulario, TipoPesos p,
                   TipoVetoresBO VetoresBaseOffset,
                   FILE *ArqComprimido, int MaxCompCod)
{
    Apontador Pos;
    TipoChave Chave;
    int Codigo, c;

    do {
        ExtraiProximaPalavra(Palavra, Indice, Linha, ArqTxt, Alfabeto);
        memcpy(Chave, Palavra, sizeof(TipoChave));

        if (*Palavra != '\0') {
            Pos = Pesquisa(Chave, p, Vocabulario);
            Codigo = Codifica(VetoresBaseOffset,
                              Vocabulario[Pos].Ordem, &c, MaxCompCod);
            Escreve(ArqComprimido, &Codigo, &c);

            do {
                ExtraiProximaPalavra(Palavra, Indice, Linha,
                                     ArqTxt, Alfabeto);

                /* O primeiro espaço depois da palavra nao e codificado */
                if (strcmp(Trim(Palavra), "") &&
                    (*Trim(Palavra)) != (char)0) {
                    memcpy(Chave, Palavra, sizeof(TipoChave));
                    Pos = Pesquisa(Chave, p, Vocabulario);
                    Codigo = Codifica(VetoresBaseOffset,
                                      Vocabulario[Pos].Ordem, &c, MaxCompCod);
                    Escreve(ArqComprimido, &Codigo, &c);
                }
            } while (strcmp(Palavra, ""));
        }
    } while (*Palavra != '\0');
}

// codigo para fazer compressão
void Compressao(FILE *ArqTxt, FILE *ArqAlf, FILE *ArqComprimido)
{
    TipoAlfabeto Alfabeto;
    TipoPalavra Palavra, Linha;
    int Ind = 0; int MaxCompCod;
    TipoDicionario Vocabulario; // = (TipoDicionario)calloc(M+1, sizeof(TipoItem))
    TipoPesos p; TipoVetoresBO VetoresBaseOffset; //(TipoVetoresBO)calloc(MAXTAMVETORESDO+1, sizeof(TipoBaseOffset))

    /* Inicializacao do Alfabeto */
    DefineAlfabeto(Alfabeto, ArqAlf); /* Le alfab. definido em arquivo */
    *Linha = '\0';
    /* Inicializacao do Vocabulario */
Inicializa(Vocabulario);
GeraPesos(p);

/* Inicio da Compressao */
PrimeiraEtapa(ArqTxt, Alfabeto, &Ind,
              Palavra, Linha, Vocabulario, p);

MaxCompCod = SegundaEtapa(Vocabulario, VetoresBaseOffset,
                          p, ArqComprimido);

fseek(ArqTxt, 0, SEEK_SET); // MUITO IMPORTANTE: Volta ao início do arquivo de texto
    Ind = 0;                   // Reinicializa o índice para 0
    *Linha = '\0';             // Limpa o buffer de linha

TerceiraEtapa(ArqTxt, Alfabeto, &Ind, Palavra, Linha,
              Vocabulario, p, VetoresBaseOffset,
              ArqComprimido, MaxCompCod);
}
