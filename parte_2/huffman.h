#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./utils/hash-table.h"

#define MAX_CODE 256

typedef struct Huffman_node{
    char* key;
    int freq;
    struct Huffman_node *left, *right;
} Huffman_node;

typedef struct NodeList{
    Huffman_node *node;
    struct NodeList *next;
}NodeList;

void free_node_list(NodeList *list);
Huffman_node *create_node(char* key, int freq, Huffman_node* left, Huffman_node* right);
NodeList* insert_node_list(NodeList* head, Huffman_node* node);
NodeList* build_list_hash(Table* freq_table);
Huffman_node* build_tree(NodeList* list);
void generate_codes(Huffman_node* root, char* prefix, Table* code_table);
void get_word_frequency(Table *t, char *text);
void save_table_in_file(FILE* out, Table* code_table);
void compress(FILE* out, char* text, Table* code_table);
void free_huffman_tree(Huffman_node* node);
void encode(FILE *in, FILE *out);
void decode(FILE *in, FILE *out);