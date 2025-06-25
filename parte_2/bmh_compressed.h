#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./utils/hash-table.h"

#define MAX_CODE 256

char* convert_word_to_code(Table* code_table, const char* word);
int bmh_search(const char* text, const char* pattern);
void read_code_table(FILE* compressed, Table* code_table);
void search_in_compressed(FILE* compressed_file, const char* palavra_original);