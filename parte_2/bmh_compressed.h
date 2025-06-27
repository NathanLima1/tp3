#ifndef BMH_COMPRESSED_H
#define BMH_COMPRESSED_H

#include "huffman_header.h"
#include "alfabeto.h"
#include "huffman_marc.h"

int BMH_busca_ocorrencias(unsigned char *T, int n,
                          unsigned char *P, int m,
                          int *ocorrencias);

void BuscaMultiplosPadroes(FILE *ArqComprimido, FILE *ArqAlf,
                            FILE *ArqPadroes, FILE *ArqSaida);

#endif