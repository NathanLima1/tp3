#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


char* read_word(FILE* fp, int* pos);

int count_lines(FILE* fp);

void write_index(FILE* fp, int pos, char index);