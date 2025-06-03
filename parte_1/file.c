#include "file.h"

void write_index(FILE* fp, int pos, char index){
    fseek(fp, pos, SEEK_SET);
    fputc(index, fp);
}

char* read_word(FILE* fp, int* pos){
    int len = 100;
    int c;
    int i = 0;

    char* word = malloc(len);
    fseek(fp, *pos, SEEK_SET);

    while ((c = fgetc(fp)) != EOF && c != ' ' && c != '\n' && c != '\r') {
        if(i >= len - 1){
            len *= 2;
            char* temp = realloc(word, len);
            if (temp == NULL) {
                free(word);
                return NULL;
            }
            word = temp;
        }
        word[i++] = (char)c;
    }
    word[i] = '\0';
    *pos = ftell(fp);
    return word;
}

int count_lines(FILE* fp){
    int lines = 0;
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            lines++;
        }
    }
    return lines;
}