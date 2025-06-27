#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "bmh_decompressed.h"
#include "./utils/file.h"

int main(int argc, char *argv[]){
    FILE* fp_texto = NULL;
    FILE* fp_padrao = NULL;
    FILE* fp_saida = NULL;
    FILE* fp_comprimido = NULL;

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    

    int option = 1;

    char* input_file1 = "texto.txt";
    char* pattern_file = "padrao.txt";
    char* output_file = "saida.txt";
    int opt;

    while((opt = getopt(argc, argv, "a:b:o:f:")) > 0){
        switch(opt){
            case 'a':
                input_file1 = optarg;
                break;
            case 'b': 
                pattern_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            default:
                printf("Entrada inválida, use -a:-b:-o\n");
                return 0;
        }
    }

    

    fp_texto = fopen(input_file1, "r");
    fp_padrao = fopen(pattern_file, "r");
    fp_saida = fopen(output_file, "w");
    char *texto = read_file(input_file1);
    
    // Comeca a medir o tempo de usuario //
    struct rusage usage_start, usage_end;
    getrusage(RUSAGE_SELF, &usage_start);


    // Linha a linha de padrão
    char padrao[1024];
    while(fgets(padrao, sizeof(padrao), fp_padrao) != NULL) {
        if (strlen(padrao) > 0) {
            padrao[strcspn(padrao, "\n")] = '\0';

            printf("\nProcurando pelo padrao: %s\n\n", padrao);

            // search_in_compressed(fp_comprimido, padrao); // Busca no arquivo comprimido
            bmh(padrao, texto, strlen(padrao), strlen(texto), fp_saida); // BMH normal
        }
    }
    free(texto);
    


    fclose(fp_saida);
    // fclose(fp_texto);
    fclose(fp_padrao);
    free(texto);



    // Termina medição //
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


    return 0;
}