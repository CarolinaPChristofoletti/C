#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct entrada {
    int x;
    long ini;
    long fim;
} entrada;

typedef struct no {
    entrada *ent; // Dar free depois
    no **filhos;// Dar free depois
    int n;// Contador de entradas em uso
    bool folha; // É folha?
} no;

void inserir(int x, long ini, long fim){

}

void buscar(no* root, int x){
    // Função aqui
    printf("%d não esta na arvore\n", x);
}

void imprimir_niveis(no* root){
    // Imprime os códigos da árvore por níveis

}

void imprimir_linha(no* root){

}

void df_free(no* root){
    // Passa fazendo free em filhos e entrada.
    // Depois, dá free em root
}

int main(void){
    no* root = NULL;
    char caractere[2];
    int x;
    long ini, fim;
    scanf("%s", &caractere);
    while(strcmp(caractere, "f")!=0){
        if(strcmp(caractere, "i")!=0){
            scanf("%d %ld %ld", &x, &ini, &fim);
            inserir(x, ini, fim);
        }
        else if(strcmp(caractere, "b")!=0){
            scanf("%d", &x);
            buscar(root, x);
        }
        else if(strcmp(caractere, "l")!=0){
            imprimir_linha(root);
        }
        else {
            imprimir_niveis(root);
        }
    }
    df_free(root);
    return 0;
}