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

#define MAX_LEN 10000
#define MAX_OCCURRENCES 1000

int main(int argc, char *argv[]){
    FILE* fp1 = stdin;
    FILE* fp2 = stdin;
    FILE* fp_out = stdin;

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    int option = 1;

    char* input_file1 = "entrada1.txt";
    char* input_file2 = "entrada2.txt";
    char* output_file = "saida.txt";
    int opt;

    while((opt = getopt(argc, argv, "a:b:o:f:")) > 0){
        switch(opt){
            case 'a':
                input_file1 = optarg;
                break;
            case 'b': 
                input_file2 = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'f':
                option = atoi(optarg);
                if(option == 2){
                    printf("Shift-And selecionado.\n");
                }else{
                    printf("Programação dinâmica selecionado.\n");
                }
                break;
            default:
                printf("Entrada inválida, use -a:-b:-o:-f\n");
                return 0;
        }
    }

    fp1 = fopen(input_file1, "r");
    fp2 = fopen(input_file2, "r");

    if(fp1 == NULL || fp2 == NULL){
        perror("Erro ao abrir o(s) arquivo(s)");
        return 1;
    }

    fp_out = fopen(output_file, "w");

    // Comeca a medir o tempo de usuario
    struct rusage usage_start, usage_end;
    getrusage(RUSAGE_SELF, &usage_start);

    char *text = malloc(MAX_LEN * sizeof(char));
    if (!fgets(text, MAX_LEN, fp1)) {
        fprintf(stderr, "Erro ao ler o texto do arquivo.\n");
        fclose(fp1);
        fclose(fp2);
        fclose(fp_out);
        return 1;
    }
    text[strcspn(text, "\n")] = 0;
    fclose(fp1);


    char pattern[MAX_LEN];
    while (fgets(pattern, MAX_LEN, fp2)) {
        pattern[strcspn(pattern, "\n")] = 0;

        for (int k = 0; k <= 3; k++) {
            int positions[MAX_OCCURRENCES];
            int count;
            levenshtein(text, pattern, k, positions, &count);

            fprintf(fp_out, "%s", pattern);
            for (int i = 0; i < count; i++)
                fprintf(fp_out, " %d", positions[i]);
            fprintf(fp_out, "\n");

        }
    }

    free(text);
    fclose(fp2);
    fclose(fp_out);

    // Termina medição
    gettimeofday(&end_time, NULL);
    getrusage(RUSAGE_SELF, &usage_end);

    // Calculando o tempo real (tempo total de execução)
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long microseconds = end_time.tv_usec - start_time.tv_usec;
    if (microseconds < 0) {
        seconds--;
        microseconds += 1000000;
    }

    // Calculando tempo de usuário e sistema
    long user_time_sec = usage_end.ru_utime.tv_sec - usage_start.ru_utime.tv_sec;
    long user_time_usec = usage_end.ru_utime.tv_usec - usage_start.ru_utime.tv_usec;

    long sys_time_sec = usage_end.ru_stime.tv_sec - usage_start.ru_stime.tv_sec;
    long sys_time_usec = usage_end.ru_stime.tv_usec - usage_start.ru_stime.tv_usec;

    if (user_time_usec < 0) {
        user_time_sec--;
        user_time_usec += 1000000;
    }

    if (sys_time_usec < 0) {
        sys_time_sec--;
        sys_time_usec += 1000000;
    }

    printf("Tempo Real: %ld.%06ld segundos\n", seconds, microseconds);
    printf("Tempo de Usuário: %ld.%06ld segundos\n", user_time_sec, user_time_usec);
    printf("Tempo de Sistema: %ld.%06ld segundos\n", sys_time_sec, sys_time_usec);

    fclose(fp1);
    fclose(fp2);
    fclose(fp_out);

    return 0;
}