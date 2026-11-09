#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

typedef struct entrada {
    int x;
    long ini;
    long fim;
} entrada;

typedef struct no {
    entrada *ent; // Dar free depois
    struct no **filhos;// Dar free depois
    int n;// Contador de entradas em uso
    bool folha; // É folha?
} no;

bool buscar_bool(no* root, int x){
    int i=0;
    if(root == NULL) return false;
    while(i < root->n && root->ent[i].x < x){
        i++;
    }
    if(i<root->n && root->ent[i].x==x){
        return true;
    }
    if(root->folha == true){
        return false;
    }
    return buscar_bool(root->filhos[i], x);
}

no* criar_no(int t, bool folha){
    // Em árvores B, a inserção é sempre feita nas folhas
    no* novo = malloc(sizeof(no));
    novo -> ent = malloc((2*t-1)*sizeof(entrada)); // O máximo de chaves por nó em uma árvore B é 2t-1
    novo -> filhos = malloc(2*t * sizeof(no *));
    novo -> n = 0;
    novo -> folha = folha;
    for(int i=0; i< 2*t; i++){
        novo -> filhos[i] = NULL;
    }
    return novo;
}

void dividir_filho(no* pai, int i, int t){
// Pai não está cheio e o filho[i] está
    no *y = pai->filhos[i];
    no *z = criar_no(t, y->folha);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->ent[j] = y->ent[j + t];
    if (!y->folha)
        for (int j = 0; j < t; j++)
            z->filhos[j] = y->filhos[j + t];

    for (int j = pai->n; j > i; j--)
        pai->filhos[j + 1] = pai->filhos[j];
    pai->filhos[i + 1] = z;

    for (int j = pai->n - 1; j >= i; j--)
        pai->ent[j + 1] = pai->ent[j];
    pai->ent[i] = y->ent[t - 1];
    pai->n++;

    y->n = t - 1;
}

void inserir_nao_cheio(int t, no *nope_cheio, entrada e){
    int i = nope_cheio->n - 1;

    if (nope_cheio->folha) {
        while (i >= 0 && e.x < nope_cheio->ent[i].x) {
            nope_cheio->ent[i + 1] = nope_cheio->ent[i];
            i--;
        }
        nope_cheio->ent[i + 1] = e;
        nope_cheio->n++;
    } else {
        while (i >= 0 && e.x < nope_cheio->ent[i].x)
            i--;
        i++;
        if (nope_cheio->filhos[i]->n == 2*t - 1) {
            dividir_filho(nope_cheio, i, t);
            if (e.x > nope_cheio->ent[i].x)
                i++;
        }
        inserir_nao_cheio(t, nope_cheio->filhos[i], e);
    }
}

void inserir(no** root, int t, int x, long ini, long fim){
    // Busca é feita antes para que o split só seja
    // feito se a chave ainda não estiver na árvore.
    bool esta_arvore = buscar_bool((*root), x);
    if(esta_arvore==true) return;

    entrada e = {x, ini, fim};
    if(*root == NULL){
        *root = criar_no(t, true);
        (*root)->ent[0] = e;
        (*root)->n=1;
        return;
    }
    if((*root)->n==2*t-1){
        no *s = criar_no(t, false);
        s->filhos[0] = *root;
        *root = s;
        dividir_filho(s, 0, t);
    }
    inserir_nao_cheio(t, *root, e);
}

void buscar(no* root, int x){
    if(root == NULL){
        printf("%d nao esta na arvore\n", x);
        return;
    }
    int i=0;
    while(i < root->n && root->ent[i].x < x){
        i++;
    }
    if(i<root->n && root->ent[i].x==x){
        printf("%d: %ld %ld\n", root->ent[i].x, root->ent[i].ini, root->ent[i].fim);
        return;
    }
    if(root->folha == true){
        printf("%d nao esta na arvore\n", x);
        return;
    }
    buscar(root->filhos[i], x);
}

int contar_nos(no* root){
    if(root == NULL) return 0;
    int total = 1;
    if(root->folha==false){
        for(int i=0; i<=root->n; i++){
            total += contar_nos(root->filhos[i]);
        }
    }
    return total;
}

void imprimir_niveis(no* root){
    // Imprime os códigos da árvore por níveis
    if(root==NULL){
        printf("A arvore esta vazia.\n");
        return;
    }
    int total = contar_nos(root);
    no **fila = malloc(total * sizeof(no *));
    int inicio = 0, fim = 0;
    int nivel = 0;
    fila[fim++] = root;
    while(inicio<fim){
        int quantidade = fim - inicio;
        printf("nivel %d: ", nivel);
        for(int k=0;k<quantidade;k++){
            no *atual = fila[inicio++];
            printf("[");
            for(int i = 0; i<atual->n; i++){
                if(i>0)printf(",");
                printf("%d", atual->ent[i].x);
            }
            printf("] ");
            if(atual->folha==false){
                for(int i=0; i<=atual->n; i++){
                    fila[fim++] = atual->filhos[i];
                }
            }
        }
        printf("\n");
        nivel++;
    }
    free(fila);    
}

void imprimir_linha(no* root){
    // Imprime os códigos da árvore em uma linha ordenada
    if(root==NULL) return;
    for(int i = 0; i < root->n; i++){
        if(root->folha!=true) imprimir_linha(root->filhos[i]); // subárvore antes da chave i
        printf("%d ", root->ent[i].x);
    }
    if(root->folha!=true) imprimir_linha(root->filhos[root->n]); // último filho
}

void df_free(no* root){
    // Passa fazenope_cheioo free em filhos e entrada.
    // Depois, dá free em root
    if(root==NULL) return;
    if(root->folha==false){
        for(int i=0; i<=root->n; i++){
            df_free(root->filhos[i]);
        }
    }
    free(root->ent);
    free(root->filhos);
    free(root);
}

int main(void){
    no* root = NULL;
    char caractere[2];
    int x, t=0;
    long ini, fim;
    scanf("%s", caractere);
    while(strcmp(caractere, "f")!=0){
        if(strcmp(caractere, "c") == 0){
            df_free(root);
            scanf("%d", &t);
            root = NULL;
        }
        else if(strcmp(caractere, "i") == 0){
            scanf("%d %ld %ld", &x, &ini, &fim);
            inserir(&root, t, x, ini, fim);
        }
        else if(strcmp(caractere, "b") == 0){
            scanf("%d", &x);
            buscar(root, x);
        }
        else if(strcmp(caractere, "l") == 0){
            imprimir_linha(root); printf("\n");
        }
        else {
            imprimir_niveis(root);
        }
        scanf("%s", caractere);
    }
    df_free(root);
    return 0;
}