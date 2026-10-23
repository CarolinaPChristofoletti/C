/**
    Declarações para uma palavra de um dicionário armazenada 
    em um vetor dinâmico.
**/


#ifndef Index_H
#define Index_H

typedef struct Index{
    long identificador; 
    char *visibilidade;  
    char *estado;  
    int altura;
    struct Index* left; 
    struct Index* right; 
} Index;

void df_free(Index* D);
int df_insert(Index** head, long x, char* visibilidade, char* estado);
int df_atualizar(Index** head, long x, char* visibilidade, char* estado);
int df_search(Index* head, long x);
void df_print(Index* head);
void df_print_estados(Index* head);

#endif