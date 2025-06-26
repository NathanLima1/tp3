#ifndef HUFFMAN_MARC_H
#define HUFFMAN_MARC_H

#include "huffman_header.h"
#include "alfabeto.h"
#include "descompressao.h"

void CalculaCompCodigo(TipoDicionario A, int n);
int Codifica(TipoVetoresBO vetores_base_offset, int ordem, int*c, int max_com_cod);
int Decodifica(TipoVetoresBO vetores_base_offset, FILE* arq_comprimido, int max_comp_cod);
int ConstroiVetores(TipoVetoresBO VetoresBaseOffset,
                    TipoDicionario Vocabulario,
                    int n, FILE *ArqComprimido);
int LeNumInt(FILE *ArqComprimido);
void GravaNumInt(FILE *ArqComprimido, int Num);
Indice OrdenaPorFrequencia(TipoDicionario Vocabulario);
void Escreve(FILE *ArqComprimido, int *Codigo, int *c);
void Busca(FILE *ArqComprimido, FILE *ArqAlf);
void Atribui(TipoPadrao P, int Codigo, int c);

#endif