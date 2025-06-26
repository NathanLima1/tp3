#ifndef TIPOS_H
#define TIPOS_H

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_WORD        256
#define M               4096      /* tamanho da tabela de hash   */
#define VAZIO           ""        /* marca uma posição vazia     */

typedef char  TipoChave[MAX_WORD];

typedef struct {
    TipoChave Chave;
    int       Freq;   /* frequência da palavra       */
    int       Ordem;  /* posição entre as folhas HF  */
} TipoItem;

typedef TipoItem         TipoDicionario[M + 1];
typedef unsigned int     TipoPesos[MAX_WORD];

typedef struct { int Base, Offset; } TipoBaseOffset;
#define MaxTamVetoresBO  10
typedef TipoBaseOffset   TipoVetoresBO[MaxTamVetoresBO + 1];

typedef char             TipoPalavra[MAX_WORD];
typedef TipoPalavra      TipoVetorPalavra[M + 1];

typedef unsigned char    TipoPadrao[MaxTamVetoresBO + 1];

#define Vazio ""
typedef int Indice;
typedef int Apontador;

#define MAX_TEXTO 2000000
typedef unsigned char    TipoTexto[MAX_TEXTO];

void Inicializa (TipoDicionario D);
void GeraPesos  (TipoPesos p);
int  Pesquisa   (TipoChave Ch, TipoPesos p, TipoDicionario D);
void Insere     (TipoItem x,   TipoPesos p, TipoDicionario D);

char *Trim(char *s);

void QuickSort (TipoDicionario A, int *n);

void BMH (unsigned char *T, int n,
          unsigned char *P, int m); 

#endif 
