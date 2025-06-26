#include "huffman_marc.h"

// Generalização do calculo dos comprimentos dos códigos
void CalculaCompCodigo(TipoDicionario A, int n){
    int u = 0; // Nodos internos usados
    int h = 0; // Altura da arvore
    int NoInt; // Numero de nodos internos

    int prox, raiz, folha;
    int disp = 1;
    int x, resto = 0;

    if(n > BaseNum -1){
        resto = 1 + ((n-resto)%(BaseNum -1));
    }else{
        resto = n-1;
    }

    NoInt = 1 + ((n - resto)/BaseNum -1);

    if(resto < 2) resto = BaseNum;

    for(x = n-1; x>= (n-resto)+1;x--) A[n].Freq += A[x].Freq;

    /*Primeira Fase*/
    raiz = n;
    folha = n-resto;

    for(prox = n-1; prox >= (n-NoInt)+1; prox--){
        // Procura posição
        if(folha < 1 || (raiz > prox && A[raiz].Freq <= A[folha].Freq)){
            // Nó interno
            A[prox].Freq = A[raiz].Freq;
            A[raiz].Freq = prox;
            raiz--;
        }else{
            // Nó folha
            A[prox].Freq = A[folha].Freq;
            folha--;
        }

        // Atualiza frequencias
        for(x = 1; x <= BaseNum - 1; x++){
            if(folha < 1 || (raiz > prox && A[raiz].Freq < A[folha].Freq)){
                //No interno
                A[prox].Freq += A[raiz].Freq;
                A[raiz].Freq = prox;
                raiz--;
            }else{
                // No folha
                A[prox].Freq += A[folha].Freq; 
                folha--;
            }
        }
    }

    /*Segunda Fase*/
    A[prox + 1].Freq = 0;
    raiz = prox + 1;
    
    for(prox = raiz + 1; prox <= n; prox++){
        A[prox].Freq = A[A[prox].Freq].Freq + 1;
    }

    /*Terceira Fase*/
    prox = 1;
    while(disp > 0){
        while(raiz <= n && A[raiz].Freq == h){
            u++;
            raiz++;
        }
        while(disp > u){
            A[prox].Freq = h;
            prox++;
            disp--;
            if(prox > n){
                u = 0;
                break;
            }
        }
        disp = BaseNum*u;
        h++;
        u = 0;
    }
}

// Codificacao orientada a bytes
int Codifica(TipoVetoresBO vetores_base_offset, int ordem, int*c, int max_com_cod){
    *c = 1;
    while(ordem > vetores_base_offset[*c + 1].Offset && *c + 1 <= max_com_cod) (*c)++;

    return (ordem - vetores_base_offset[*c].Offset + vetores_base_offset[*c].Base);
}

// Decodificacao orientada a bytes
int Decodifica(TipoVetoresBO vetores_base_offset, FILE* arq_comprimido, int max_comp_cod){
    int c = 1;
    int codigo = 0, codigo_tmp = 0;
    int log_base2 = (int)round(log(BaseNum)/log(2.0));
    if (fread(&codigo, sizeof(unsigned char), 1, arq_comprimido) != 1) {
        fprintf(stderr, "Erro ao ler byte de código\n");
        return -1; // ou um código de erro apropriado
    }

    codigo -= 128; // remove o bit de marcação

    while((c+1 <= max_comp_cod) &&
     ((codigo << log_base2) >= vetores_base_offset[c+1].Base)){
        if (fread(&codigo_tmp, sizeof(unsigned char), 1, arq_comprimido) != 1) {
            fprintf(stderr, "Erro ao ler byte de código temporário\n");
            break;
        }
        codigo = (codigo << log_base2) | codigo_tmp;
        c++;
    }

    return (codigo - vetores_base_offset[c].Base + vetores_base_offset[c].Offset);
}

// construção dos vetores base e offset
int ConstroiVetores(TipoVetoresBO VetoresBaseOffset,
                    TipoDicionario Vocabulario,
                    int n, FILE *ArqComprimido)
{ 
    int Ws[MaxTamVetoresBO + 1];
    int i, MaxCompCod;
    MaxCompCod = Vocabulario[n].Freq;
    
    for (i = 1; i <= MaxCompCod; i++) {
        Ws[i] = 0; 
        VetoresBaseOffset[i].Offset = 0;
    }

    for (i = 1; i <= n; i++) {
        Ws[Vocabulario[i].Freq]++;
        VetoresBaseOffset[Vocabulario[i].Freq].Offset =
            i - Ws[Vocabulario[i].Freq] + 1;
    }

    VetoresBaseOffset[1].Base = 0;

    for (i = 2; i <= MaxCompCod; i++) {
        VetoresBaseOffset[i].Base =
            BaseNum * (VetoresBaseOffset[i-1].Base + Ws[i-1]);
        if (VetoresBaseOffset[i].Offset == 0)
            VetoresBaseOffset[i].Offset = VetoresBaseOffset[i-1].Offset;
    }

    /* Salvando as tabelas em disco */
    GravaNumInt(ArqComprimido, MaxCompCod);

    for (i = 1; i <= MaxCompCod; i++) {
        GravaNumInt(ArqComprimido, VetoresBaseOffset[i].Base);
        GravaNumInt(ArqComprimido, VetoresBaseOffset[i].Offset);
    }

    return MaxCompCod;
}

int LeNumInt(FILE *ArqComprimido)
{ 
    int Num;
    if(fread(&Num, sizeof(int), 1, ArqComprimido) != 1){
         fprintf(stderr, "Erro ao ler num\n");
        return -1;
    }
    return Num;
}

void GravaNumInt(FILE *ArqComprimido, int Num)
{ 
    fwrite(&Num, sizeof(int), 1, ArqComprimido); 
}

// Função para ordenar o vocabulario por frequencia
Indice OrdenaPorFrequencia(TipoDicionario Vocabulario)
{
    Indice i; 
    Indice n = 1;
    TipoItem Item;
    Item = Vocabulario[1];

    for (i = 0; i <= M - 1; i++)
    {
        if (strcmp(Vocabulario[i].Chave, Vazio))
        {
            if (i != 1) {
                Vocabulario[n] = Vocabulario[i];
                n++;
            }
        }
    }

    if (strcmp(Item.Chave, Vazio))
        Vocabulario[n] = Item;
    else
        n--;

    QuickSort(Vocabulario, &n);

    return n;
}


// escreve o código no arquivo comprimido
void Escreve(FILE *ArqComprimido, int *Codigo, int *c)
{
    unsigned char Saida[MaxTamVetoresBO + 1];
    int i = 1; int cTmp;
    cTmp = *c;
    Saida[i] = (((unsigned)(*Codigo)) >> ((*c - 1) * 8 - *c + 1)) | 0x80;
    i++;
    while (i <= *c)
    {
        Saida[i] = (((unsigned)(*Codigo)) >> ((*c - i) * 8 - *c + 1));
        i++;
    }

    for (i = 1; i <= cTmp; i++)
        fwrite(&Saida[i], sizeof(unsigned char), 1, ArqComprimido);
}

// função para realizar busca no arwuivo comprimido
void Busca(FILE *ArqComprimido, FILE *ArqAlf)
{
    TipoAlfabeto Alfabeto;
    int Ind, Codigo, MaxCompCod;
    TipoVetorPalavra Vocabulario;
    TipoVetoresBO VetoresBaseOffset;
    TipoPalavra p;
    TipoTexto T;
    TipoPadrao Padrao;
    int c, Ord = 0, NumNodosFolhas;
    int n = 1;

    DefineAlfabeto(Alfabeto, ArqAlf);  /* Lê alfabeto definido em arq. */
    MaxCompCod = LeVetores(ArqComprimido, VetoresBaseOffset);
    NumNodosFolhas = LeVocabulario(ArqComprimido, Vocabulario);

    while (fread(&T[n], sizeof(char), 1, ArqComprimido)) n++;

    do {
        printf("Padrao:");
        if (fgets(p, MaxAlfabeto + 1, stdin) == NULL) {
            fprintf(stderr, "Erro ao ler padrão de entrada\n");
            continue;
        }
        p[strlen(p) - 1] = '\0';

        for (Ind = 1; Ind <= NumNodosFolhas; Ind++)
            if (!strcmp(Vocabulario[Ind], p)) Ord = Ind;

        Codigo = Codifica(VetoresBaseOffset, Ord, &c, MaxCompCod);
        Atribui(Padrao, Codigo, c);
        BMH(T, n, Padrao, c);

    } while (strcmp(p, "s"));
}

// função para atribuir o código ao padrao
void Atribui(TipoPadrao P, int Codigo, int c)
{
   int i = 1;

   P[i] = (char)((Codigo >> ((c - 1) * 8 - c + 1)) | 0x80);
   i++;  c--;

   while (c > 0)
   {
      P[i] = (char)(Codigo >> ((c - 1) * 8 - c + 1));
      i++;  c--;
   }
}