#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

/*
Duas tabelas distintas T1 e T2
p1 = h1(x) - olha em T1[p1] e depois em T2[p1], nessa ordem. 
Se 1 ocupada, x expulsa y e olha p2. Se z, y expulsa z e olha em t1 
se está vazio. Sequência continua alternando entre T1 e T2.

Existe a chance disso gerar um looping infinito, então é necessário
deixar um contador de expulsões com um limite pré-definido.
A tabela é redimensionada sempre que o fator de carga exceder um 
limite pré-definido.
*/

typedef struct par{
    int chave;
    int valor;
} par;

par* criar_tabela(int n){
    // Cria tabelas com n pares
    par* tabela = (par *)malloc(n*sizeof(par));
    for(int i=0; i<n; i++){
        tabela[i].chave = -1;
    }
    return tabela;
}

int h1(int x, int n){
    int posicao = (((2*x+1)%11)%n);
    return posicao;
}

int h2(int x, int n){
    int posicao = (((3*x+2)%7)%n);
    return posicao;
}

void redimensionar(int* tabela, int n){
    // FALTA
}

void verificar_tamanho(int n, int ocupacao, int* tabela){
    if(ocupacao>=0.7*n){
        redimensionar(tabela, n); // PARA QUAL TAMANHO ELA DEVE SER REDIMENSIONADA? Devo redimensionar 
        // só uma ou as duas, redistribuindo as chaves?
    }
}

int buscar_chave(par* tabela1, par* tabela2, int chave, int n){
    if(tabela1[h1(chave, n)].chave == -1) return -1, -1;
    else if(tabela1[h1(chave, n)].chave == chave) return chave, tabela1[h1(chave, n)].valor;
    else if (tabela2[h2(chave, n)].chave == -1) return -1, -1;
    else if (tabela2[h2(chave, n)].chave == chave) return chave, tabela2[h2(chave, n)].valor;
    return -1;
}

void inserir_chave(par*tabela1, par* tabela2, int chave, int valor, int limite, 
int* ocupacao1, int* ocupacao2, int n){
    // Insere o par (chave, valor). Cada chave deve ser inserida 
    // uma única vez VERIFICAR SE CHAVE JÁ NÃO ESTÁ NA TABELA
    int contador = 0;
    while(contador<=limite){
        if(tabela1[h1(chave, n)].chave == -1){
            tabela1[h1(chave, n)].chave = chave;
            tabela1[h1(chave, n)].valor = valor;
            verificar_tamanho(n, ocupacao1, tabela1);
            return;
        }
        else if(tabela2[h2(chave, n)].chave == -1){
            tabela2[h2(chave, n)].chave = chave;
            tabela2[h2(chave, n)].valor = valor;
            verificar_tamanho(n, ocupacao2, tabela2);
            return;
        }
        // Falta colocar os casos em que as chaves já estão lá e os 
        // que tem outras chaves no mesmo lugar.
    }
    redimensionar(tabela1, n);
    redimensionar(tabela2, n);
}

void remover_chave(par* tabela1, par* tabela2, int chave, int n){
    // Remove o par com a chave dada da tabela
    // Como na função de busca apenas a chave é verificada, não 
    // é necessário alterar o valor ligado a ela.
    if(tabela1[h1(chave, n)].chave == -1){
        return;
    }
    else if(tabela1[h1(chave, n)].chave == chave){
        tabela1[h1(chave, n)].chave = -1;
        return;
    }
    else if (tabela2[h2(chave, n)].chave == -1){
        return;
    }
    else if (tabela2[h2(chave, n)].chave == chave){
        tabela2[h2(chave, n)].chave = -1;
        return;
    }
}

void imprimir_linha(int* tabela1, int* tabela2, int chave, int n){
    int chave1, resposta = buscar_chave(tabela1, tabela2, chave, n);
    if(chave1 == -1) printf("%d -> !E\n", chave);
    else printf("%d -> %d\n", chave, resposta);
}

void df_free(int* tabela, int n){
    // Libera a memória armazenada dinamicamente em uma tabela de hashing.
    for(int i=0; i<n; i++){
        free(tabela[i]);
    }
}

int main(void){
    int chave=0, valor=0;
    char caractere;
    int *ocupacao1, *ocupacao2;
    (*ocupacao1) = 0;
    (*ocupacao2) = 0;
    int limite; // Qual é esse limite?
    int n=0;
    int *tabela1 = NULL, *tabela2 = NULL;
    scanf("%c", &caractere);
    while(caractere!="f"){
        if(caractere=="c"){
            df_free(tabela1, n);
            df_free(tabela2, n);
            scanf("%d", &n);
            limite = 2*n;
            tabela1 = criar_tabela(n);
            tabela2 = criar_tabela(n);
        }
        else if(caractere=="i"){
            scanf("%d %d", &chave, &valor);
            inserir_chave(tabela1, tabela2, chave, valor, limite, ocupacao1, ocupacao2, n);
        }
        else if(caractere=="r"){
            remover_chave(tabela1, tabela2, chave, n);
        }
        else{
            imprimir_linha(tabela1, tabela2, chave, n);
        }
    }
    return 0;
}