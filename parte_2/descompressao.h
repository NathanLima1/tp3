#ifndef DESCOMPRESSAO_H
#define DESCOMPRESSAO_H

#include "huffman_header.h"

void Descompressao(FILE *ArqComprimido, FILE *ArqTxt, FILE *ArqAlf);
int LeVetores(FILE *ArqComprimido, TipoBaseOffset *VetoresBaseOffset);
int LeVocabulario(FILE *ArqComprimido, TipoVetorPalavra Vocabulario);

#endif