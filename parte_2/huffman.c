#include "huffman.h"


Huffman_node *create_node(char* key, int freq, Huffman_node* left, Huffman_node* right){
    Huffman_node *node = malloc(sizeof(Huffman_node));
    node->key = key ? strdup(key) : NULL;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

NodeList* insert_node_list(NodeList* head, Huffman_node* node){
    NodeList* new_node = malloc(sizeof(NodeList));
    new_node->node = node;
    new_node->next = NULL;

    if(!head || node->freq < head->node->freq){
        new_node->next = head;
        return new_node;
    }

    NodeList *atual = head;
    while(atual->next && atual->next->node->freq <= node->freq){
        atual = atual->next;
    }

    new_node->next = atual->next;
    atual->next = new_node;
    return head;
}

void free_node_list(NodeList *list) {
    while (list) {
        NodeList *tmp = list;
        list = list->next;
        free(tmp);
    }
}

NodeList* build_list_hash(Table* freq_table){
    NodeList* list = NULL;
    for(int i = 0; i < freq_table->size; i++){
        tableItem *item = freq_table->data[i];
        while(item){
            list = insert_node_list(list, create_node(item->key, item->value, NULL, NULL));
            item = item->next;
        }
    }
    return list;
}
Huffman_node* build_tree(NodeList* list){
    while(list && list->next){
        Huffman_node* a = list->node;
        Huffman_node* b = list->next->node;
        list = list->next->next;
        Huffman_node* merged = create_node(NULL, a->freq + b->freq, a, b);
        list = insert_node_list(list, merged);
    }
    Huffman_node* root = list ? list->node : NULL;

    free(list);
    return root;
}

void generate_codes(Huffman_node* root, char* prefix, Table* code_table){
    if(!root) return;

    if (!root->left && !root->right && root->key) {
    insert(code_table, root->key);
    tableItem* item = get_item(code_table, root->key);

    if (item->key) free(item->key);
    item->key = strdup(root->key);

    if (item->valueStr) free(item->valueStr);
    item->valueStr = strdup(prefix);

    return;
    }

    char left[MAX_CODE], right[MAX_CODE];
    sprintf(left, "%s0", prefix);
    sprintf(right, "%s1", prefix);

    generate_codes(root->left, left, code_table);
    generate_codes(root->right, right, code_table);
}


void get_word_frequency(Table *t, char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        char s[2] = {text[i], '\0'};
        if (get_item(t, s)) {
            update_value(t, s, 1);
        } else {
            insert_with_value(t, s, 1);
        }
    }
}

void save_table_in_file(FILE* out, Table* code_table){
    for (int i = 0; i < code_table->size; i++) {
        tableItem *item = code_table->data[i];
        while (item) {
            fprintf(out, "%s:%s\n", item->key, item->valueStr);
            item = item->next;
        }
    }
}

void compress(FILE* out, char* text, Table* code_table){
    for (int i = 0; text[i] != '\0'; i++) {
        char s[2] = {text[i], '\0'};
        tableItem* item = get_item(code_table, s);
        if (item) fprintf(out, "%s", item->valueStr);
    }
}

void free_huffman_tree(Huffman_node* node) {
    if (!node) return;
    free_huffman_tree(node->left);
    free_huffman_tree(node->right);
    if (node->key) free(node->key);
    free(node);
}


void encode(FILE *in, FILE *out) {
    // mede o comprimento do arquivo ao mover seu ponteiro e após volta para o inicio
    fseek(in, 0, SEEK_END);
    long size = ftell(in);
    rewind(in);

    char *text = malloc(size + 1);
    size_t bytes_read = fread(text, 1, size, in);
    if (bytes_read != size) {
        fprintf(stderr, "Aviso: fread leu apenas %zu de %ld bytes em encode\n", bytes_read, size);
    }
    text[size] = '\0';

    Table *freq_table = initTable(256);
    get_word_frequency(freq_table, text);

    NodeList *list = build_list_hash(freq_table);
    Huffman_node *tree = build_tree(list);

    Table *code_table = initTable(256);
    char prefix[MAX_CODE] = "";
    generate_codes(tree, prefix, code_table);

    save_table_in_file(out, code_table);

    fprintf(out, "%%\n"); // marcador de fim da marcação

    compress(out, text, code_table);

    free(text);
    freeTable(freq_table);
    freeTable(code_table);
    free_huffman_tree(tree);
    free_node_list(list);
}


void decode(FILE *in, FILE *out) {
    Table *decode_table = initTable(256);
    char line[1024];

    while (fgets(line, sizeof(line), in)) {
        if (strcmp(line, "%\n") == 0) break; // fim da marcação
        char *sep = strchr(line, ':');
        if (sep) {
            *sep = '\0';
            char *key = line;
            char *value = sep + 1;
            value[strcspn(value, "\n")] = '\0';
            insert_with_value(decode_table, value, key[0]);
        }
    }

    // Le o texto comprimido
    char bitstring[8192] = {0};
    size_t bytes_read = fread(bitstring, 1, sizeof(bitstring), in);
    if(bytes_read == 0){
        fprintf(stderr, "Aviso: fread não leu nada na funcao decode\n");
    }

    //Decodificar
    char buffer[MAX_CODE] = "";
    int pos = 0;

    for (int i = 0; bitstring[i] == '0' || bitstring[i] == '1'; i++) {
        buffer[pos++] = bitstring[i];
        buffer[pos] = '\0';

        tableItem *item = get_item(decode_table, buffer);
        if (item) {
            fputc(item->value, out);
            pos = 0;
            buffer[0] = '\0';
        }
    }

    freeTable(decode_table);
}

