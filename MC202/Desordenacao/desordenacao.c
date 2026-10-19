#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Ordena um vetor de inteiros
int ordenar_vetor(const void *a, const void *b){
    return (*(int*)a-*(int*)b);
}

// Computa uma medida de desordenação
int medir_desordenacao(char cadeia[]){
    int desordenacao = 0;
    for(int i=0; i<strlen(cadeia); i++){
        for(int k=i+1; k<strlen(cadeia); k++){
            if(cadeia[i]>cadeia[k]){
                desordenacao++;
            }
        }
    }
    return desordenacao;
}

int main(void){
    int c;
    scanf("%d", &c);
    for(int i = 0; i<c; i++){
        int k, j;
        scanf("%d %d", &j, &k);
        int desordenacoes[k];
        char cadeias[k][j+1];
        for(int a=0; a<k; a++){
            scanf("%s", cadeias[a]);
            int desordenacao = medir_desordenacao(cadeias[a]);
            desordenacoes[a] = desordenacao;
        }
        int des_ordenadas[k];
        for(int z=0; z<k; z++){
            des_ordenadas[z] = desordenacoes[z];
        }
        qsort(des_ordenadas, k, sizeof(int), ordenar_vetor);
        int *visitei = (int *)calloc(k, sizeof(int));
        for(int a=0; a<k; a++){
            int b = 0;
            while(desordenacoes[b] != des_ordenadas[a] || visitei[b] == 1){
                b++;
            }
            visitei[b] = 1;
            printf("%s\n", cadeias[b]);
        }
        printf("\n");
        free(visitei);
    }
    return 0;
}