#ifndef COMPRESSAO_H
#define COMPRESSAO_H

#include "huffman_header.h"
#include "alfabeto.h"
#include "huffman_marc.h"

void PrimeiraEtapa(FILE *ArqTxt, TipoAlfabeto Alfabeto, int *Indice,
                   TipoPalavra Palavra, char *Linha,
                   TipoDicionario Vocabulario, TipoPesos p);

int SegundaEtapa (TipoDicionario Vocabulario,
                  TipoVetoresBO VetoresBaseOffset,
                  TipoPesos p,
                  FILE *ArqComprimido);

void TerceiraEtapa(FILE *ArqTxt, TipoAlfabeto Alfabeto, int *Indice,
                   TipoPalavra Palavra, char *Linha,
                   TipoDicionario Vocabulario, TipoPesos p,
                   TipoVetoresBO VetoresBaseOffset,
                   FILE *ArqComprimido, int MaxCompCod);

void Compressao(FILE *ArqTxt, FILE *ArqAlf, FILE *ArqComprimido);
#endif