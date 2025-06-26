#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash-table.h"

char *strdup(const char *s) {
    char *copy = malloc(strlen(s) + 1);
    if (copy) strcpy(copy, s);
    return copy;
}

unsigned int hash(char *s) {
    int len = strlen(s);
    unsigned int hash = 1;
    for (int i = 0; i < len; i++) {
        hash = hash * 33 + s[i];
    }

    return hash;
}


void converte_hash_para_dicionario(Table *hash, TipoDicionario *A, int *n) {
    int idx = 1;
    for (int i = 0; i < hash->size; i++) {
        tableItem *curr = hash->data[i];
        while (curr) {
            strcpy(A[idx].simbolo, curr->key);
            A[idx].Freq = curr->value;
            idx++;
            curr = curr->next;
        }
    }
    *n = idx - 1;  // número real de elementos
}

Table *initTable(int size) {
    Table *t = malloc(sizeof(Table));
    t->data = calloc(size, sizeof(tableItem*));
    t->size = size;
    return t;
}

void insert_with_value(Table *t, const char *key, int value) {
    int index = hash((char *)key) % t->size;
    tableItem *item = malloc(sizeof(tableItem));
    item->key = strdup(key);
    item->value = value;
    item->valueStr = NULL; // ainda não tem código
    item->next = t->data[index];
    t->data[index] = item;
}

void insert_with_string(Table *t, const char *key, const char *valueStr) {
    int index = hash((char *)key) % t->size;
    tableItem *item = malloc(sizeof(tableItem));
    item->key = strdup(key);
    item->value = 0;
    item->valueStr = strdup(valueStr); // salva o código Huffman
    item->next = t->data[index];
    t->data[index] = item;
}

void update_value(Table *t, const char *key, int delta) {
    tableItem *item = get_item(t, key);
    if (item) {
        item->value += delta;
    }
}

void insert(Table *t, char *key) {
    int index = hash(key) % t->size;
    tableItem *crr_item = t->data[index];

    while (crr_item != NULL) {
        if (strcmp(crr_item->key, key) == 0) {
            crr_item->value++;
            return;
        }
        crr_item = crr_item->next;
    }

    tableItem *item = malloc(sizeof(tableItem));
    item->key = strdup(key);
    item->value = 1;
    item->valueStr = NULL;
    item->next = t->data[index];
    t->data[index] = item;
}

void freeTableItem(tableItem *item) {
    if (item == NULL) return;
    freeTableItem(item->next);
    free(item->key);
    if (item->valueStr) free(item->valueStr);
    free(item);
}


void freeTable(Table *t) {
    for (int i = 0; i < t->size; i++) {
        freeTableItem(t->data[i]);
    }
    free(t->data);
    free(t);
}

void printTableFormatted(Table *t) {
    printf("+--------+----------------------+--------+\n");
    printf("| Index  | Key                  | Value  |\n");
    printf("+--------+----------------------+--------+\n");

    for (int i = 0; i < t->size; i++) {
        tableItem *curr = t->data[i];
        while (curr) {
            printf("| %-6d | %-20s | %-6d |\n", i, curr->key, curr->value);
            curr = curr->next;
        }
    }

    printf("+--------+----------------------+--------+\n");
}

tableItem *get_item(Table *t, const char *key) {
    int index = hash((char *)key) % t->size;
    tableItem *curr = t->data[index];

    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL; // não encontrado
}

// int main() {
//     Table *t = initTable(5);
//     insert(t, "pato\0");
//     printTableFormatted(t);
//     return 0;
// }