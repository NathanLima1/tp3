#include "bmh_compressed.h"

char* convert_word_to_code(Table* code_table, const char* word) {
    char* encoded = malloc(MAX_CODE * strlen(word));
    encoded[0] = '\0';

    for (int i = 0; word[i] != '\0'; i++) {
        char s[2] = {word[i], '\0'};
        tableItem* item = get_item(code_table, s);
        if (!item) {
            fprintf(stderr, "Caractere '%c' não encontrado na tabela de códigos.\n", word[i]);
            free(encoded);
            return NULL;
        }
        strcat(encoded, item->valueStr);
    }
    return encoded;
}

int bmh_search(const char* text, const char* pattern) {
    int n = strlen(text);
    int m = strlen(pattern);
    if (m > n) return -1;

    int shift[256];
    for (int i = 0; i < 256; i++) shift[i] = m;
    for (int i = 0; i < m - 1; i++) shift[(unsigned char)pattern[i]] = m - 1 - i;

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[i + j]) j--;

        if (j < 0) return i;  // Encontrado

        i += shift[(unsigned char)text[i + m - 1]];
    }
    return -1;  // Não encontrado
}

void read_code_table(FILE* compressed, Table* code_table){
    char line[1024];

    while (fgets(line, sizeof(line), compressed)) {
        if (strcmp(line, "%\n") == 0) break;
        char* sep = strchr(line, ':');
        if (sep) {
            *sep = '\0';
            char* key = line;
            char* value = sep + 1;
            value[strcspn(value, "\n")] = '\0';
            insert_with_string(code_table, key, value);  // key = letra, value = código
        }
    }
}

void search_in_compressed(FILE* compressed_file, const char* palavra_original) {
    Table* code_table = initTable(256);
    read_code_table(compressed_file, code_table);

    // Converte a palavra original para Huffman
    char* codificada = convert_word_to_code(code_table, palavra_original);
    if (!codificada) {
        freeTable(code_table);
        return;
    }

    char texto_comprimido[8192] = {0};
    size_t read = fread(texto_comprimido, 1, sizeof(texto_comprimido) - 1, compressed_file);
    if (read == 0) {
        fprintf(stderr, "Erro: fread não leu dados de compressed_file\n");
    }

    int pos = bmh_search(texto_comprimido, codificada);

    if (pos != -1)
        printf("Padrão encontrado na posição %d.\n", pos);
    else
        printf("Padrão não encontrado no arquivo comprimido.\n");

    free(codificada);
    freeTable(code_table);
}
