#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "funcoes_index.h"



/**
Libera a memória alocada dinamicamente para o dicionário.
**/
void df_free(Index* D){
    if (D == NULL){
        return;
    }

    df_free(D->left);
    df_free(D->right);

    if (D->visibilidade != NULL){
        free(D->visibilidade);
    }
    if (D->estado != NULL){
        free(D->estado);
    }
    free(D);
}

/*
Obtém a altura de um nó. Usada para evitar receber NULL
ao tentar acessar diretamente a altura de um nó.
*/
int altura(Index* indice) {
    return indice == NULL ? 0 : indice->altura;
}

/*
Para facilitar a leitura quando for determinar o
máximo entre dois números.
*/
int max(int a, int b) {
    return a > b ? a : b;
}

/*
Atualizando a altura de um nó
*/
void atualizar(Index* indice) {
    indice->altura = 1 + max(altura(indice->left), altura(indice->right));
}

/*
Calcula o fator de desbalanceamento de um nó
*/
int fator(Index* indice) {
    return altura(indice->left) - altura(indice->right);
}

/*
Rotações para a esquerda
*/
Index* rot_esq(Index* indice1){
    Index* indice2 = indice1->right;
    indice1->right = indice2->left;
    indice2->left = indice1;
    atualizar(indice1);  
    atualizar(indice2);
    return indice2;
}

/*
Rotações para a direita
*/
Index* rot_dir(Index* indice1){
    Index* indice2 = indice1->left;
    indice1->left = indice2->right;
    indice2->right = indice1;
    atualizar(indice1);  
    atualizar(indice2);
    return indice2;
}

/*
Balanceia o dicionário
*/
Index* balancear(Index* no) {
    atualizar(no);
    int f = fator(no);

    if (f > 1) {                          
        if (fator(no->left) < 0)          
            no->left = rot_esq(no->left);
        return rot_dir(no);
    }

    if (f < -1) {                       
        if (fator(no->right) > 0)        
            no->right = rot_dir(no->right);
        return rot_esq(no);
    }

    return no;                         
}

/**
Adiciona a palavra w com frequência f a sua devida posição no dicionário.
**/
int df_insert(Index** head, long x, char* nivel, char* estado) {
    if (*head == NULL) {
        Index* novo = malloc(sizeof(Index));
        if (novo == NULL) return 0;
        novo->visibilidade = malloc(strlen(nivel) + 1);
        if (novo->visibilidade == NULL) {
            free(novo);
            return 0;
        }
        novo->estado = malloc(strlen(estado) + 1);
        if (novo->estado == NULL) {
            free(novo);
            return 0;
        }
        strcpy(novo->visibilidade, nivel);
        strcpy(novo->estado, estado);
        novo->identificador = x;
        novo->left = NULL;
        novo->right = NULL;
        novo->altura = 1;
        *head = novo;
        return 1;
    }

    // Ignorando se o x já estiver no índice
    int ap = (*head) -> identificador;
    if(ap == x) return 1;

    int r;
    if (ap < x) r = df_insert(&(*head)->left, x, nivel, estado);
    else        r = df_insert(&(*head)->right, x, nivel, estado);

    *head = balancear(*head);
    return r;
}

/*
Atualiza o documento com identificador x, modificando sua visibilidade
e nível. Se x não estiver no índex, a operação é ignorada.
*/
int df_atualizar(Index** head, long x, char* visibilidade, char* estado){
    if (*head == NULL) {
        return 0;
    }

    int ap = (*head) -> identificador;
    if(ap == x){
        free((*head)->visibilidade);
        free((*head)->estado);
        strcpy((*head)->visibilidade, visibilidade);
        strcpy((*head)->estado, estado);
        (*head)->identificador = x;
        return 1;
    }

    int r;
    if (ap < x) r = df_atualizar(&(*head)->left, x, visibilidade, estado);
    else        r = df_atualizar(&(*head)->right, x, visibilidade, estado);

    return r;
}

/*Imprime a palavra x e sua frequência. Se não houver a 
palavra no dicionário, imprime uma mensagem de erro.*/
int df_search(Index* head, long x){
    Index* apoio = head;
    while(apoio!=NULL){
        if(apoio->identificador>x){
            apoio = apoio ->left;
        } else if(apoio->identificador<x){
            apoio = apoio -> right;
        } else {
            printf("%ld, %s, %s\n", apoio->identificador, apoio->visibilidade, apoio->estado);
            return 1; 
        }
    }
    printf("%ld não existe\n", x);
    return 0;
}

/*
Imprime os identificadores dos documentos da árvore em ordem
crescente (busca in-order) em uma única linha separados por expaço.
Se o índice estiver vazio, imprime índice vazio.

Exemṕlo de saída:
documentos: 1 2 3 .........
*/
void imprimir_indices(Index* head){
    if(head == NULL){
        return;
    }
    imprimir_indices(head->left);
    printf("%ld ", head->identificador);
    imprimir_indices(head->right);
}

void df_print(Index* head){
    if(head==NULL){
        printf("indice vazio\n");
        return;
    }
    printf("documentos: ");
    imprimir_indices(head);
    printf("\n");
}

/*
Imprime o número de documentos caragorizando por estado.

Exemplo de saída:
atualizado: 1, desatualizado: 2, obsoleto: 3
*/

void contar_estados(Index* head, int* atualizado, int* desatualizado, int* obsoleto){
    if(head==NULL){
        return;
    }
    contar_estados(head->left, atualizado, desatualizado, obsoleto);
    if(strcmp(head->estado, "atualizado")==0){
        (*atualizado)++;
    } else if(strcmp(head->estado, "desatualizado")==0){
        (*desatualizado)++;
    }else{
        (*obsoleto)++;
    }
    contar_estados(head->right, atualizado, desatualizado, obsoleto);
}

void df_print_estados(Index* head){
    int atualizado=0, desatualizado=0, obsoleto=0;
    contar_estados(head, &atualizado, &desatualizado, &obsoleto);
    printf("atualizado: %d, desatualizado: %d, obsoleto: %d\n", atualizado, desatualizado, obsoleto);
}