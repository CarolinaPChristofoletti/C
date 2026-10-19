#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;

    while (scanf("%d", &n) == 1 && n > 0) {
        int *V     = malloc(n * sizeof(int));
        int *esq   = malloc(n * sizeof(int));
        int *dir   = malloc(n * sizeof(int));
        int *pilha = malloc(n * sizeof(int));
        int *fila  = malloc(n * sizeof(int));

        if (!V || !esq || !dir || !pilha || !fila) {
            return 1;
        }

        for (int i = 0; i < n; i++) {
            if (scanf("%d", &V[i]) != 1) {
                return 1;
            }
            esq[i] = dir[i] = -1;
        }

        /* Construção O(n) com pilha */
        int topo = 0;
        for (int i = 0; i < n; i++) {
            int ultimo = -1;

            /* só remove elementos estritamente maiores:
               em empate, o índice da esquerda continua como ancestral */
            while (topo > 0 && V[pilha[topo - 1]] > V[i]) {
                ultimo = pilha[--topo];
            }

            esq[i] = ultimo;
            if (topo > 0) {
                dir[pilha[topo - 1]] = i;
            }
            pilha[topo++] = i;
        }

        int raiz = pilha[0];

        /* Percurso em largura */
        int inicio = 0, fim = 0;
        fila[fim++] = raiz;

        while (inicio < fim) {
            int quantidade = fim - inicio;

            for (int i = 0; i < quantidade; i++) {
                int atual = fila[inicio++];

                if (i > 0) printf(" ");
                printf("%d", atual);

                if (esq[atual] != -1) fila[fim++] = esq[atual];
                if (dir[atual] != -1) fila[fim++] = dir[atual];
            }
            printf("\n");
        }
        printf("\n");

        free(V); free(esq); free(dir); free(pilha); free(fila);
    }

    return 0;
}