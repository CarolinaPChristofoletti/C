/*
A função uniao recebe as tabelas A (tamanho n) e B
(tamanho m) e cria u,a tabela nova C (tamanho t) com
todas as chaves de A e B, sem repetir nenhuma. A 
inserção em C usa hashing duplo.
*/

#include <stdlib.h>

int h1(int k);
int h2(int k);

static void inserir(int *C, int t, int k, int livre) {
    long long base  = ((h1(k) % t) + t) % t;
    long long passo = ((h2(k) % t) + t) % t;
    if (passo == 0) passo = 1;           /* proteção */

    for (int i = 0; i < t; i++) {
        int pos = (int)((base + i * passo) % t);
        if (C[pos] == k) return;         /* já existe */
        if (C[pos] == livre) { C[pos] = k; return; }
    }
}

int *uniao(const int *A, int n, const int *B, int m, int t) {
    const int livre    = (int)(1u << (sizeof(int) * 8 - 1));  /* INT_MIN */
    const int removido = livre + 1;                           /* INT_MIN + 1 */

    int *C = malloc(t * sizeof(int));
    if (!C) return NULL;
    for (int i = 0; i < t; i++) C[i] = livre;

    for (int i = 0; i < n; i++)
        if (A[i] != livre && A[i] != removido) inserir(C, t, A[i], livre);
    for (int i = 0; i < m; i++)
        if (B[i] != livre && B[i] != removido) inserir(C, t, B[i], livre);

    return C;
}