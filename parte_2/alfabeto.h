#ifndef ALFABETO_H
#define ALFABETO_H

#include "huffman_header.h"

void DefineAlfabeto(TipoAlfabeto Alfabeto, FILE* ArqAlf);
void ExtraiProximaPalavra(TipoPalavra Result, int *Indice, char *Linha,
                          FILE *ArqTxt, TipoAlfabeto Alfabeto);

#endif