#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./utils/hash-table.h"

#define MAX_CODE 256

// nó arvore de Huffman
typedef struct Huffman_node{
    char* key;
    int freq;
    struct Huffman_node *left, *right;
} Huffman_node;

Huffman_node *create_node(char* key, int freq, Huffman_node* left, Huffman_node* right){
    Huffman_node *node = malloc(sizeof(Huffman_node));
    node->key = key ? strdup(key) : NULL;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

typedef struct NodeList{
    Huffman_node *node;
    struct NodeList *next;
}NodeList;

NodeList* insert_node_list(NodeList* head, Huffman_node* node){
    NodeList* new_node = malloc(sizeof(NodeList));
    new_node->node = node;
    new_node->next = NULL;

    if(!head || node->freq < head->node->freq){
        new_node->next = head;
        return new_node;
    }

    NodeList *atual = head;
    while(atual->next && atual->next->node->freq <= node->freq){
        atual = atual->next;
    }

    new_node->next = atual->next;
    atual->next = new_node;
    return head;
}

NodeList* build_list_hash(Table* freq_table){
    NodeList* list = NULL;
    for(int i = 0; i < freq_table->size; i++){
        tableItem *item = freq_table->data[i];
        while(item){
            list = insert_node_list(list, create_node(item->key, item->value, NULL, NULL));
            item = item->next;
        }
    }
    return list;
}
Huffman_node* build_tree(NodeList* list){
    while(list && list->next){
        Huffman_node* a = list->node;
        Huffman_node* b = list->next->node;
        list = list->next->next;
        Huffman_node* merged = create_node(NULL, a->freq + b->freq, a, b);
        list = insert_node_list(list, merged);
    }
    return list ? list->node : NULL;
}

void generate_codes(Huffman_node* root, char* prefix, Table* code_table){
    if(!root) return;

    if(!root->left && !root->right && root->key){
        insert(code_table, root->key);
        tableItem* item = get_item(code_table, root->key);
        free(item->key);
        item->key = strdup(root->key);
        free(item->valueStr);
        item->valueStr = strdup(prefix);
        return;
    }

    char left[MAX_CODE], right[MAX_CODE];
    sprintf(left, "%s0", prefix);
    sprintf(right, "%s1", prefix);

    generate_codes(root->left, left, code_table);
    generate_codes(root->right, right, code_table);
}

void compress_text(FILE* in, FILE* out, Table* code_table){
    int c;
    while((c = fgetc(in)) != EOF){
        char s[2] = {c, '\0'};
        tableItem* item = get_item(code_table, s);
        if(item) fprintf(out, "%s|", item->valueStr);
    }
}

void get_word_frequency(Table *t, char *text) {

}

void encode() {

}

void decode() {

}

int main() {
    return 0;
}