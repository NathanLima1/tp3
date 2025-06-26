#ifndef HUFFMAN_HEADER_H
#define HUFFMAN_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "./utils/tipos.h"

#define BaseNum 128 /*Base numerica que o algoritmo trabalha*/
#define MaxAlfabeto 255 /*Const. usada em ExtraiProximaPalavra*/
#define MaxTamVetoresBO 10
#define TRUE 1
#define FALSE 0


typedef short  TipoAlfabeto[MaxAlfabeto + 1];

typedef char       TipoPalavra[256];
typedef TipoPalavra TipoVetorPalavra[M + 1];

#endif