#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    int indice;
    struct no *esq;
    struct no *dir;
} no;


/* Encontra a posição do menor elemento de V[l..r].
   Em caso de empate, permanece o índice mais à esquerda. */
int descobrir_menor(int *V, int l, int r) {
    int indice = l;

    for (int i = l + 1; i <= r; i++) {
        if (V[i] < V[indice]) {
            indice = i;
        }
    }

    return indice;
}


/* Constrói recursivamente a árvore cartesiana canônica
   correspondente ao intervalo V[l..r]. */
no *construir(int *V, int l, int r) {
    if (l > r) {
        return NULL;
    }

    int pos = descobrir_menor(V, l, r);

    no *novo = malloc(sizeof(no));

    if (novo == NULL) {
        exit(1);
    }

    novo->indice = pos;
    novo->esq = construir(V, l, pos - 1);
    novo->dir = construir(V, pos + 1, r);

    return novo;
}


/* Percorre a árvore em largura e libera os nós. */
void imprimir_em_largura(no *raiz, int n) {
    if (raiz == NULL) {
        printf("\n");
        return;
    }

    no **fila = malloc(n * sizeof(no *));

    if (fila == NULL) {
        exit(1);
    }

    int inicio = 0;
    int fim = 0;

    fila[fim++] = raiz;

    while (inicio < fim) {
        int quantidade = fim - inicio;

        for (int i = 0; i < quantidade; i++) {
            no *atual = fila[inicio++];

            if (i > 0) {
                printf(" ");
            }

            printf("%d", atual->indice);

            if (atual->esq != NULL) {
                fila[fim++] = atual->esq;
            }

            if (atual->dir != NULL) {
                fila[fim++] = atual->dir;
            }
        }

        printf("\n");
    }

    /* Linha em branco entre árvores */
    printf("\n");

    for (int i = 0; i < fim; i++) {
        free(fila[i]);
    }

    free(fila);
}

int main(void) {
    int n;

    scanf("%d", &n);

    while (n != 0) {
        int *V = malloc(n * sizeof(int));

        if (V == NULL) {
            return 1;
        }

        for (int i = 0; i < n; i++) {
            scanf("%d", &V[i]);
        }

        no *raiz = construir(V, 0, n - 1);

        imprimir_em_largura(raiz, n);

        free(V);

        scanf("%d", &n);
    }

    return 0;
}