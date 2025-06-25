#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct tableItem {
    char *key;
    int value;
    char* valueStr;
    struct tableItem *next;
} tableItem;

typedef struct {
    tableItem **data;
    int size;
} Table;

Table *initTable(int size);
char *strdup(const char *s);
void insert(Table *t, char *key); // Adiciona um elemento na tabela e incrementa sua frequência em 1
unsigned int hash(char *s); // Gera um hash com base em uma string
void freeTableItem(tableItem *item); // Função recursiva para liberar cada índice do vetor da tabela hash
void freeTable(Table *t); // Função para liberar a tabela hash
tableItem *get_item(Table *t, const char *key);
void insert_with_value(Table *t, const char *key, int value);
void insert_with_string(Table *t, const char *key, const char *valueStr);
void update_value(Table *t, const char *key, int delta);
// Remover depois
void printTableFormatted(Table *t);

#endif