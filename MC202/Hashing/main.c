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

par *tabela1 = NULL, *tabela2 = NULL;
int n = 0;
int ocupacao1 = 0, ocupacao2 = 0;

void inserir_chave(int chave, int valor, int limite);
void redimensionar(void);
void verificar_tamanho(void);

par* criar_tabela(int n){
    // Cria tabelas com n pares
    par* tabela = (par *)malloc(n*sizeof(par));
    for(int i=0; i<n; i++){
        tabela[i].chave = -1;
    }
    return tabela;
}

void df_free(par* tabela, int n){
    // Libera a memória armazenada dinamicamente em uma tabela de hashing.
    free(tabela); // Não precisa fazer um looping, pois a tabela foi inteira alocada com um malloc
}

long long h1(int x, int n){
    // x é a chave e n é o tamanho da tabela
    long long posicao = (((2LL*x+1)%1000000007LL)%n);
    return posicao;
}

long long h2(int x, int n){
    long long posicao = (((3LL*x+2)%998244353LL)%n);
    return posicao;
}


void verificar_tamanho(void){
    if(ocupacao1>=0.7*n || ocupacao2 >= 0.7*n){
        redimensionar();
    }
}

void inserir_chave(int chave, int valor, int limite){
    // Insere o par (chave, valor). Cada chave deve ser inserida
    // uma única vez VERIFICAR SE CHAVE JÁ NÃO ESTÁ NA TABELA
    int contador;
    inicio_geral:
    contador = 0;
    while(contador<=limite){
        // Verifica se a chave está em alguma das tabelas
        if(tabela1[h1(chave, n)].chave == chave){
            tabela1[h1(chave, n)].valor = valor;
            return;
        }
        else if(tabela2[h2(chave, n)].chave == chave){
            tabela2[h2(chave, n)].valor = valor;
            return;
        }

        // Verifica se a posição analisada está vazia na tabela 1
        int p = h1(chave, n);
        if(tabela1[p].chave == -1 || tabela1[p].chave == -2){
            tabela1[p].chave = chave;
            tabela1[p].valor = valor;
            ocupacao1++;
            verificar_tamanho();
            return;
        }

        int chave_apoio = tabela1[p].chave;
        int valor_apoio = tabela1[p].valor;
        tabela1[p].chave = chave;
        tabela1[p].valor = valor;
        chave = chave_apoio;
        valor = valor_apoio;
        contador++;

        p = h2(chave, n);
        if(tabela2[p].chave == -1 || tabela2[p].chave == -2){
            tabela2[p].chave = chave;
            tabela2[p].valor = valor;
            ocupacao2++;
            verificar_tamanho();
            return;
        }

        chave_apoio = tabela2[p].chave;
        valor_apoio = tabela2[p].valor;
        tabela2[p].chave = chave;
        tabela2[p].valor = valor;
        chave = chave_apoio;
        valor = valor_apoio;
        contador++;
    }
    redimensionar();
    goto inicio_geral;
}


void redimensionar(void){
    par* antiga1 = tabela1;
    par* antiga2 = tabela2;
    int n_antigo = n;
    n = 2*n_antigo;
    tabela1 = criar_tabela(n);
    tabela2 = criar_tabela(n);
    ocupacao1 = 0;
    ocupacao2 = 0;
    for(int i = 0; i<n_antigo; i++){
        if(antiga1[i].chave != -1 && antiga1[i].chave != -2){
            inserir_chave(antiga1[i].chave, antiga1[i].valor, 15);
        }
        if(antiga2[i].chave != -1 && antiga2[i].chave != -2){
            inserir_chave(antiga2[i].chave, antiga2[i].valor, 15);
        }
    }
    df_free(antiga1, n_antigo);
    df_free(antiga2, n_antigo);
    return;
}

int buscar_chave(int chave, int* valor){
    if(tabela1[h1(chave, n)].chave == chave){
        *valor = tabela1[h1(chave, n)].valor;
        return 1;
    }
    else if (tabela2[h2(chave, n)].chave == chave){
        *valor = tabela2[h2(chave, n)].valor;
        return 1;
    }
    return 0;
}

void remover_chave(int chave){
    // Remove o par com a chave dada da tabela
    // Como na função de busca apenas a chave é verificada, não
    // é necessário alterar o valor ligado a ela.
    if(tabela1[h1(chave, n)].chave == chave){
        tabela1[h1(chave, n)].chave = -2;
        ocupacao1--;
        return;
    }
    else if (tabela2[h2(chave, n)].chave == chave){
        tabela2[h2(chave, n)].chave = -2;
        ocupacao2--;
        return;
    }
}

void imprimir_linha(int chave){
    int valor;
    if(!buscar_chave(chave, &valor)) printf("%d -> !E\n", chave);
    else printf("%d -> %d\n", chave, valor);
}

int main(void){
    int chave=0, valor=0;
    char caractere;
    int LIMITE = 15;
    while(scanf(" %c", &caractere)==1 && caractere!='f'){
        if(caractere=='c'){
            df_free(tabela1, n);
            df_free(tabela2, n);
            scanf("%d", &n);
            tabela1 = criar_tabela(n);
            tabela2 = criar_tabela(n);
            ocupacao1 = 0;
            ocupacao2 = 0;
        }
        else if(caractere=='i'){
            scanf("%d %d", &chave, &valor);
            inserir_chave(chave, valor, LIMITE);
        }
        else if(caractere=='r'){
            scanf("%d", &chave);
            remover_chave(chave);
        }
        else if(caractere=='b'){
            scanf("%d", &chave);
            imprimir_linha(chave);
        }
    }
    df_free(tabela1, n);
    df_free(tabela2, n);
    return 0;
}