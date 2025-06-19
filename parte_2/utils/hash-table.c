#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash-table.h"

unsigned int hash(char *s) {
    int len = strlen(s);
    unsigned int hash = 1;
    for (int i = 0; i < len; i++) {
        hash = hash * 33 + s[i];
    }

    return hash;
}

Table *initTable(int size) {
    Table *t = malloc(sizeof(Table));
    t->data = calloc(size, sizeof(tableItem*));
    t->size = size;
    return t;
}
int insert(Table *t, char *key) {
    int index = hash(key) % t->size;
    tableItem *crr_item = t->data[index];

    while (crr_item != NULL) {
        if (strcmp(crr_item->key, key) == 0) {
            crr_item->value++;
            return 0;
        }
        crr_item = crr_item->next;
    }

    tableItem *item = malloc(sizeof(tableItem));
    item->key = strdup(key);
    item->value = 1;
    item->next = t->data[index];
    t->data[index] = item;
}

void freeTableItem(tableItem *item) {
    if (item == NULL) return;
    freeTableItem(item->next);
    free(item->key);
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

// int main() {
//     Table *t = initTable(5);
//     insert(t, "pato\0");
//     printTableFormatted(t);
//     return 0;
// }