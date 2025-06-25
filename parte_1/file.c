#include "file.h"


// Função auxiliar para ler todo o conteúdo de um arquivo
char* read_file(const char* nome) {
    FILE* f = fopen(nome, "rb");
    if (!f) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    rewind(f);

    if (tam == 0) { // arquivo vazio
        fclose(f);
        char* vazio = malloc(1);
        if (vazio) vazio[0] = '\0';
        return vazio;
    }

    char* buf = malloc(tam + 1);
    if (!buf) {
        perror("Erro de alocação");
        fclose(f);
        return NULL;
    }

    size_t lidos = fread(buf, 1, tam, f);
    if (lidos != tam) {
        if (feof(f)) {
            // fim de arquivo atingido - talvez arquivo truncado
            fprintf(stderr, "Aviso: arquivo truncado ou leitura incompleta\n");
        } else {
            perror("Erro na leitura do arquivo");
            free(buf);
            fclose(f);
            return NULL;
        }
    }

    buf[lidos] = '\0';
    fclose(f);
    return buf;
}