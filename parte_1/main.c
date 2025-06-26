#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "levenshtein.h"
#include "file.h"
#include "word.h"
#include "shift_and.h"

#define MAX_PADRAO 256

int main(int argc, char *argv[]) {
    struct timeval start_time, end_time;
    struct rusage usage_start, usage_end;

    char* input_file1 = "entrada1.txt";
    char* input_file2 = "entrada2.txt";
    char* output_file = "saida.txt";
    int option = 1; // 1 = programação dinâmica, 2 = shift-and (ainda não usado)
    int k = 0;
    int opt;

    while ((opt = getopt(argc, argv, "a:b:o:f:k:")) > 0) {
        switch (opt) {
            case 'a': input_file1 = optarg; break;
            case 'b': input_file2 = optarg; break;
            case 'o': output_file = optarg; break;
            case 'f':
                option = atoi(optarg);
                if (option == 2)
                    printf("Shift-And selecionado.\n");
                else
                    printf("Programação dinâmica selecionado.\n");
                break;
            
            case 'k': k = atoi(optarg); break;

            default:
                fprintf(stderr, "Uso: %s -a <texto> -b <padroes> -o <saida> -f <1|2> -k <int>\n", argv[0]);
                return 1;
        }
    }

    gettimeofday(&start_time, NULL);
    getrusage(RUSAGE_SELF, &usage_start);

    FILE *fpads = fopen(input_file2, "r");
    FILE *fout  = fopen(output_file, "w");
    if (!fpads || !fout) {
        perror("Erro ao abrir arquivos de entrada ou saída");
        return 1;
    }

    char *texto = read_file(input_file1); // função definida em file.c
    if (!texto) {
        fprintf(stderr, "Erro ao ler o texto\n");
        return 1;
    }

    size_t n = strlen(texto);
    int *pos = malloc((n + 1) * sizeof(int));
    if (!pos) {
        perror("malloc");
        return 1;
    }

    char linha[MAX_PADRAO];
    while (fgets(linha, sizeof(linha), fpads)) {
        linha[strcspn(linha, "\r\n")] = '\0'; // remove \n ou \r
        if (*linha == '\0') continue;

        if (option == 1) {
            int cont = 0;
            levenshtein(texto, linha, k, pos, &cont);
            fprintf(fout, "%s", linha);
            for (int i = 0; i < cont; ++i)
                fprintf(fout, " %d", pos[i] + 1);
            fprintf(fout, "\n");
        } else if (option == 2) {
            int m = strlen(linha);
            int comp = shift_and(linha, texto, n, m, k, fout);
            fprintf(fout, "Comparações Shift-And: %d\n", comp);
        }
    }

    fclose(fpads);
    fclose(fout);
    free(pos);
    free(texto);

    gettimeofday(&end_time, NULL);
    getrusage(RUSAGE_SELF, &usage_end);

    long seconds = end_time.tv_sec - start_time.tv_sec;
    long microseconds = end_time.tv_usec - start_time.tv_usec;
    if (microseconds < 0) {
        seconds--;
        microseconds += 1000000;
    }

    long user_time_sec = usage_end.ru_utime.tv_sec - usage_start.ru_utime.tv_sec;
    long user_time_usec = usage_end.ru_utime.tv_usec - usage_start.ru_utime.tv_usec;
    if (user_time_usec < 0) {
        user_time_sec--;
        user_time_usec += 1000000;
    }

    long sys_time_sec = usage_end.ru_stime.tv_sec - usage_start.ru_stime.tv_sec;
    long sys_time_usec = usage_end.ru_stime.tv_usec - usage_start.ru_stime.tv_usec;
    if (sys_time_usec < 0) {
        sys_time_sec--;
        sys_time_usec += 1000000;
    }

    printf("Tempo Real: %ld.%06ld segundos\n", seconds, microseconds);
    printf("Tempo de Usuário: %ld.%06ld segundos\n", user_time_sec, user_time_usec);
    printf("Tempo de Sistema: %ld.%06ld segundos\n", sys_time_sec, sys_time_usec);

    return 0;
}
