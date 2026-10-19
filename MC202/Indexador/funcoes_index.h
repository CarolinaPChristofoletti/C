/**
    Declarações para uma palavra de um dicionário armazenada 
    em um vetor dinâmico.
**/


#ifndef DICIONARIO_H
#define DICIONARIO_H

typedef struct index{
    long identificador; 
    char *visibilidade;  
    char *estado;  
    int altura;
    struct index* left; 
    struct index* right; 
} index;

void df_free(index* D);
int df_insert(index** head, long x, char* visibilidade, char* estado);
int df_atualizar(index** head, long x, char* visibilidade, char* estado);
int df_search(index* head, long x);
void df_print(index* head);
void df_print_estados(index* head);

#endif