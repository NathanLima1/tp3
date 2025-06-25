#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

char *read_file(char *nome) {
    FILE *fp = fopen(nome, "rb");

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    char *texto = malloc(size + 1);

    rewind(fp);
    fread(texto, 1, size, fp);
    texto[size] = '\0';

    fclose(fp);
    return texto;
}