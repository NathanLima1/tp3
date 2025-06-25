#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "./utils/hash-table.h"

#define BaseNUm 128 /*Base numerica que o algoritmo trabalha*/
#define MaxAlfabeto 255 /*Const. usada em ExtraiProximaPalavra*/
#define MaxTamVetoresBO 10
#define TRUE 1
#define FALSE 0

// Generalização do calculo dos comprimentos dos códigos
void calculaCompCodigo(TipoDicionario A, int n){
    int u = 0; // Nodos internos usados
    int h = 0; // Altura da arvore
    int NoInt; // Numero de nodos internos

    int prox, raiz, folha;
    int disp = 1;
    int x, resto;

    if(n > BaseNum -1){
        resto = 1 + ((n-resto)%(BaseNum -1));
    }else{
        resto = n-1;
    }

    NoInt = 1 + ((n - resto)/BaseNum -1);

    if(resto < 2) resto = BaseNum;

    for(x = n-1; x>= (n-resto)+1;x--) A[n].Freq += A[x].Freq;

    /*Primeira Fase*/
    raiz = n;
    folha = n-resto;

    for(prox = n-1; prox >= (n-NoInt)+1; prox--){
        // Procura posição
        if(folha < 1 || raiz > prox && A[raiz].Freq <= A[folha].Freq){
            // Nó interno
            A[prox].Freq = A[raiz].Freq;
            A[raiz].Freq = prox;
            raiz--;
        }else{
            // Nó folha
            A[prox].Freq = A[folha].Freq;
            folha--;
        }

        // Atualiza frequencias
        for(x = 1; x <= BaseNum - 1; x++){
            if(folha < 1 || raiz > prox && A[raiz].Freq < A[folha].Freq){
                //No interno
                A[prox].Freq += A[raiz].Freq;
                A[raiz].Freq = prox;
                raiz--;
            }else{
                // No folha
                A[prox].Freq += A[folha].Freq; 
                folha--;
            }
        }
    }

    /*Segunda Fase*/
    A[prox + 1].Freq = 0;
    raiz = prox + 1;
    
    for(prox = raiz + 1; prox <= n; prox++){
        A[prox].Freq = A[A[prox].Freq].Freq + 1;
    }

    /*Terceira Fase*/
    prox = 1;
    while(disp > 0){
        while(raiz <= n && A[raiz].Freq == h){
            u++;
            raiz++;
        }
        while(disp > u){
            A[prox].Freq = h;
            prox++;
            disp--;
            if(prox > n){
                u = 0;
                break;
            }
        }
        disp = BaseNum*u;
        h++;
        u = 0;
    }
}

// Codificacao orientada a bytes
int codifica(TipoVetoresBO vetores_base_offset, int ordem, int*c, int max_com_cod){
    *c = 1;
    while(ordem > vetores_base_offset[*c + 1].offset && *c + 1 <= max_com_cod) (*c)++;

    return (ordem - vetores_base_offset[*c].offset + vetores_base_offset[*c].base);
}

// Decodificacao orientada a bytes
int decodifica(TipoVetoresBO vetores_base_offset, FILE* arq_comprimido, int max_comp_cod){
    int c = 1;
    int codigo = 0, codigo_tmp = 0;
    int log_base2 = (int)round(log(BaseNum)/log(2.0));
    fread(&codigo, sizeof(unsigned char), 1, arq_comprimido);

    codigo -= 128; // remove o bit de marcação

    while((c+1 <= max_comp_cod) &&
     ((codigo << log_base2) >= vetores_base_offset[c+1].base)){
        fread(&codigo_tmp, sizeof(unsigned char), 1, arq_comprimido);
        codigo = (codigo << log_base2) | codigo_tmp;
        c++;
    }

    return (codigo - vetores_base_offset[c].base + vetores_base_offset[c].offset);
}

// construção dos vetores base e offset
int ConstroiVetores(TipoVetoresBO VetoresBaseOffset,
                    TipoDicionario Vocabulario,
                    int n, FILE *ArqComprimido)
{ 
    int Ws[MaxTamVetoresBO + 1];
    int i, MaxCompCod;
    MaxCompCod = Vocabulario[n].Freq;
    
    for (i = 1; i <= MaxCompCod; i++) {
        Ws[i] = 0; 
        VetoresBaseOffset[i].Offset = 0;
    }

    for (i = 1; i <= n; i++) {
        Ws[Vocabulario[i].Freq]++;
        VetoresBaseOffset[Vocabulario[i].Freq].Offset =
            i - Ws[Vocabulario[i].Freq] + 1;
    }

    VetoresBaseOffset[1].Base = 0;

    for (i = 2; i <= MaxCompCod; i++) {
        VetoresBaseOffset[i].Base =
            BaseNum * (VetoresBaseOffset[i-1].Base + Ws[i-1]);
        if (VetoresBaseOffset[i].Offset == 0)
            VetoresBaseOffset[i].Offset = VetoresBaseOffset[i-1].Offset;
    }

    /* Salvando as tabelas em disco */
    GravaNumInt(ArqComprimido, MaxCompCod);

    for (i = 1; i <= MaxCompCod; i++) {
        GravaNumInt(ArqComprimido, VetoresBaseOffset[i].Base);
        GravaNumInt(ArqComprimido, VetoresBaseOffset[i].Offset);
    }

    return MaxCompCod;
}

int LeNumInt(FILE *ArqComprimido)
{ 
    int Num;
    fread(&Num, sizeof(int), 1, ArqComprimido);
    return Num;
}

void GravaNumInt(FILE *ArqComprimido, int Num)
{ 
    fwrite(&Num, sizeof(int), 1, ArqComprimido); 
}

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

// codigo para fazer compressão
void Compressao(FILE *ArqTxt, FILE *ArqAlf, FILE *ArqComprimido)
{
    TipoAlfabeto Alfabeto;
    TipoPalavra Palavra, Linha;
    int Ind = 1; int MaxCompCod;
    TipoDicionario Vocabulario;
    TipoPesos p; TipoVetoresBO VetoresBaseOffset;

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

fseek(ArqTxt, 0, SEEK_SET); /* move cursor para inicio do arquivo */
Ind = 1;
*Linha = '\0';

TerceiraEtapa(ArqTxt, Alfabeto, &Ind, Palavra, Linha,
              Vocabulario, p, VetoresBaseOffset,
              ArqComprimido, MaxCompCod);
}

// Proxima etapa da compressão

void PrimeiraEtapa(FILE *ArqTxt, TipoAlfabeto Alfabeto, int *Indice,
                   TipoPalavra Palavra, char *Linha,
                   TipoDicionario Vocabulario, TipoPesos p)
{
    TipoItem Elemento;
    int i;

    do {
        ExtraiProximaPalavra(Palavra, Indice, Linha, ArqTxt, Alfabeto);
        memcpy(Elemento.Chave, Palavra, sizeof(TipoChave));
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
    int Result, i, j, NumNodosFolhas, PosArq;
    TipoItem Elemento;
    char Ch;
    TipoPalavra Palavra;

    NumNodosFolhas = OrdenaPorFrequencia(Vocabulario);
    CalculaCompCodigo(Vocabulario, NumNodosFolhas);
    Result = ConstroiVetores(VetoresBaseOffset, Vocabulario,
                              NumNodosFolhas, ArqComprimido);

    /* Grava Vocabulario */
    GravaNumInt(ArqComprimido, NumNodosFolhas);
    PosArq = ftell(ArqComprimido);

    for (i = 1; i <= NumNodosFolhas; i++)
    {
        j = 1;
        while (Vocabulario[i].Chave[j - 1] != (char)0)
        {
            fwrite(&Vocabulario[i].Chave[j - 1], sizeof(char),
                   1, ArqComprimido);
            j++;
        }
        Ch = (char)0;
        fwrite(&Ch, sizeof(char), 1, ArqComprimido);
    }

    /* Lê e reconstrói a condicao de hash
       no vetor que contem o vocabulario */
    fseek(ArqComprimido, PosArq, SEEK_SET);
    Inicializa(Vocabulario);

    for (i = 1; i <= NumNodosFolhas; i++)
    {
        *Palavra = '\0';
        do
        {
            fread(&Ch, sizeof(char), 1, ArqComprimido);
            if (Ch != (char)0)
                sprintf(Palavra + strlen(Palavra), "%c", Ch);
        } while (Ch != (char)0);

        memcpy(Elemento.Chave, Palavra, sizeof(TipoChave));
        Elemento.Ordem = i;
        j = Pesquisa(Elemento.Chave, p, Vocabulario);
        if (j >= M)
            Insere(Elemento, p, Vocabulario);
    }

    return Result;
}

// Função para ordenar o vocabulario por frequencia

Indice OrdenaPorFrequencia(TipoDicionario Vocabulario)
{
    Indice i; 
    Indice n = 1;
    TipoItem Item;
    Item = Vocabulario[1];

    for (i = 0; i <= M - 1; i++)
    {
        if (strcmp(Vocabulario[i].Chave, Vazio))
        {
            if (i != 1) {
                Vocabulario[n] = Vocabulario[i];
                n++;
            }
        }
    }

    if (strcmp(Item.Chave, Vazio))
        Vocabulario[n] = Item;
    else
        n--;

    QuickSort(Vocabulario, &n);

    return n;
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

// escreve o código no arquivo comprimido
void Escreve(FILE *ArqComprimido, int *Codigo, int *c)
{
    unsigned char Saida[MaxTamVetoresBO + 1];
    int i = 1; int cTmp;
    cTmp = *c;
    Saida[i] = (((unsigned)(*Codigo)) >> ((*c - 1) * 8 - *c + 1)) | 0x80;
    i++;
    while (i <= *c)
    {
        Saida[i] = (((unsigned)(*Codigo)) >> ((*c - i) * 8 - *c + 1));
        i++;
    }

    for (i = 1; i <= cTmp; i++)
        fwrite(&Saida[i], sizeof(unsigned char), 1, ArqComprimido);
}

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

// função para realizar busca no arwuivo comprimido
void Busca(FILE *ArqComprimido, FILE *ArqAlf)
{
    TipoAlfabeto Alfabeto;
    int Ind, Codigo, MaxCompCod;
    TipoVetorPalavra Vocabulario;
    TipoVetoresBO VetoresBaseOffset;
    TipoTexto T;
    TipoPadrao Padrao;
    int c, Ord, NumNodosFolhas;
    int n = 1;

    DefineAlfabeto(Alfabeto, ArqAlf);  /* Lê alfabeto definido em arq. */
    MaxCompCod = LeVetores(ArqComprimido, VetoresBaseOffset);
    NumNodosFolhas = LeVocabulario(ArqComprimido, Vocabulario);

    while (fread(&T[n], sizeof(char), 1, ArqComprimido)) n++;

    do {
        printf("Padrao:");
        fgets(p, MaxAlfabeto + 1, stdin);
        p[strlen(p) - 1] = '\0';

        for (Ind = 1; Ind <= NumNodosFolhas; Ind++)
            if (!strcmp(Vocabulario[Ind], p)) Ord = Ind;

        Codigo = Codifica(VetoresBaseOffset, Ord, &c, MaxCompCod);
        Atribui(Padrao, Codigo, c);
        BMH(T, n, Padrao, c);

    } while (strcmp(p, "s"));
}

// função para atribuir o código ao padrao
void Atribui(TipoPadrao P, int Codigo, int c)
{
   int i = 1,
       cTmp = c;

   P[i] = (char)((Codigo >> ((c - 1) * 8 - c + 1)) | 0x80);
   i++;  c--;

   while (c > 0)
   {
      P[i] = (char)(Codigo >> ((c - 1) * 8 - c + 1));
      i++;  c--;
   }
}

/*Programa para teste dos algoritmos*/
/* —— Entram aqui os tipos do Programa E.26 —— */
/* —— Entram aqui os tipos do Programa H.1  —— */

typedef short  TipoAlfabeto[MaxAlfabeto + 1];

typedef struct {
   int Base;
   int Offset;
} TipoBaseOffset;

typedef TipoBaseOffset  TipoVetoresBO[MaxTamVetoresBO + 1];

typedef char       TipoPalavra[256];
typedef TipoPalavra TipoVetorPalavra[M + 1];

/* ——— Entra aqui a função GeraPeso do Programa E.22 ——— */
/* ——— Entra aqui a função de transformação do Programa E.23 ——— */
/* ——— Entram aqui os operadores apresentados no Programa E.27 ——— */
/* ——— Entram aqui as funções Particao e                       ——— */
/* ——— Quicksort dos Programas D.6 e D.7                       ——— */

int main(int argc, char *argv[])
{
   FILE *ArqTxt        = NULL,
        *ArqAlf        = NULL,
        *ArqComprimido = NULL;

   TipoPalavra NomeArqTxt, NomeArqAlf, NomeArqComp, Opcao;

   printf("*******************************************\n");
   printf("*                                         *\n");
   printf("*                 Opcoes                  *\n");
   printf("*                                         *\n");
   printf("* (c) Compressao                          *\n");
   printf("* (d) Descompressao                       *\n");
   printf("* (p) Pesquisa no texto comprimido        *\n");
   printf("*******************************************\n");
   printf("* Opcao: ");
   fgets(Opcao, MaxAlfabeto + 1, stdin);

   strcpy(NomeArqAlf, "alfabeto.txt");
    ArqAlf = fopen(NomeArqAlf, "r");

    if (Opcao[0] == 'c')
    {
        printf("Arquivo texto a ser comprimido:");
        fgets(NomeArqTxt, MaxAlfabeto + 1, stdin);
        NomeArqTxt[strlen(NomeArqTxt) - 1] = '\0';

        printf("Arquivo comprimido a ser gerado:");
        fgets(NomeArqComp, MaxAlfabeto + 1, stdin);
        NomeArqComp[strlen(NomeArqComp) - 1] = '\0';

        ArqTxt = fopen(NomeArqTxt, "r");
        ArqComprimido = fopen(NomeArqComp, "wb");

        Compressao(ArqTxt, ArqAlf, ArqComprimido);

        fclose(ArqTxt);
        ArqTxt = NULL;

        fclose(ArqComprimido);
        ArqComprimido = NULL;
    }
    else if (Opcao[0] == 'd')
    {
        printf("Arquivo comprimido a ser descomprimido:");
        fgets(NomeArqComp, MaxAlfabeto + 1, stdin);
        NomeArqComp[strlen(NomeArqComp) - 1] = '\0';

        printf("Arquivo texto a ser gerado:");
        fgets(NomeArqTxt, MaxAlfabeto + 1, stdin);
        NomeArqTxt[strlen(NomeArqTxt) - 1] = '\0';

        ArqTxt = fopen(NomeArqTxt, "w");
        ArqComprimido = fopen(NomeArqComp, "rb");

        Descompressao(ArqComprimido, ArqTxt, ArqAlf);

        fclose(ArqTxt);
        ArqTxt = NULL;

        fclose(ArqComprimido);
        ArqComprimido = NULL;
    }
    else if (Opcao[0] == 'p')
    {
        printf("Arquivo comprimido para ser pesquisado:");
        fgets(NomeArqComp, MaxAlfabeto + 1, stdin);
        NomeArqComp[strlen(NomeArqComp) - 1] = '\0';

        strcpy(NomeArqComp, NomeArqComp);
        ArqComprimido = fopen(NomeArqComp, "rb");

        Busca(ArqComprimido, ArqAlf);

        fclose(ArqComprimido);
        ArqComprimido = NULL;
    }

    return 0;
}