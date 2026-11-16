#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

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
    unsigned int chave;
    int valor;
} par;

// Definindo as funções antes para conseguir chamar uma dentro da outra da forma como precisava
void inserir_chave(par** tabela1, par** tabela2, unsigned int chave, int valor, int limite,
int* ocupacao1, int* ocupacao2, int* n);
void redimensionar(par** tabela1, par** tabela2, int* n, int* ocupacao1, int* ocupacao2);
void verificar_tamanho(int* n, int* ocupacao1, int* ocupacao2, par** tabela1, par** tabela2);

par* criar_tabela(int n){
    // Cria tabelas com n pares
    par* tabela = (par *)malloc(n*sizeof(par));
    for(int i=0; i<n; i++){
        tabela[i].chave = -1;
    }
    return tabela;
}

void df_free(par* tabela){
    // Libera a memória armazenada dinamicamente em uma tabela de hashing.
    free(tabela); // Não precisa fazer um looping, pois a tabela foi inteira alocada com um malloc
}

// MurMurHash
// Função adaptada de https://github.com/ksss/digest-murmurhash/blob/master/ext/digest/murmurhash/64a.c
uint64_t murmur_hash_process64a(const char *key, uint32_t len, uint64_t seed){
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    uint64_t h = seed ^ (len * m);

    const uint64_t * data = (const uint64_t *)key;
    const uint64_t * end = data + (len/8);

    while(data != end){
        uint64_t k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const unsigned char * data2 = (const unsigned char*)data;

    switch(len & 7) {
        case 7: h ^= (uint64_t)((uint64_t)data2[6] << (uint64_t)48);
        case 6: h ^= (uint64_t)((uint64_t)data2[5] << (uint64_t)40);
        case 5: h ^= (uint64_t)((uint64_t)data2[4] << (uint64_t)32);
        case 4: h ^= (uint64_t)((uint64_t)data2[3] << (uint64_t)24);
        case 3: h ^= (uint64_t)((uint64_t)data2[2] << (uint64_t)16);
        case 2: h ^= (uint64_t)((uint64_t)data2[1] << (uint64_t)8 );
        case 1: h ^= (uint64_t)((uint64_t)data2[0]                );
                h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

long long h1(int x, int n){
    // x é a chave e n é o tamanho da tabela
    uint64_t hash = murmur_hash_process64a((const char *)&x, sizeof(x), 0);
    long long posicao = (hash & 0xFFFFFFFFULL) % n;
    return posicao;
}

long long h2(int x, int n){
    uint64_t hash = murmur_hash_process64a((const char *)&x, sizeof(x), 0);
    long long posicao = (hash >> 32) % n;
    return posicao;
}

void verificar_tamanho(int* n, int* ocupacao1, int* ocupacao2, par** tabela1, par** tabela2){
    if((*ocupacao1)>=0.7*(*n) || (*ocupacao2) >= 0.7*(*n)){
        redimensionar(tabela1, tabela2, n, ocupacao1, ocupacao2);
    }
}

void inserir_chave(par** tabela1, par** tabela2, unsigned int chave, int valor, int limite,
int* ocupacao1, int* ocupacao2, int* n){
    // Insere o par (chave, valor). Cada chave deve ser inserida
    // uma única vez VERIFICAR SE CHAVE JÁ NÃO ESTÁ NA TABELA
    int contador;
    inicio_geral:
    contador = 0;
    while(contador<=limite){
        // Verifica se a chave está em alguma das tabelas
        if((*tabela1)[h1(chave, *n)].chave == chave){
            (*tabela1)[h1(chave, *n)].valor = valor;
            return;
        }
        else if((*tabela2)[h2(chave, *n)].chave == chave){
            (*tabela2)[h2(chave, *n)].valor = valor;
            return;
        }

        // Verifica se a posição analisada está vazia na tabela 1
        int p = h1(chave, *n);
        if((*tabela1)[p].chave == -1 || (*tabela1)[p].chave == -2){
            (*tabela1)[p].chave = chave;
            (*tabela1)[p].valor = valor;
            (*ocupacao1)++;
            verificar_tamanho(n, ocupacao1, ocupacao2, tabela1, tabela2);
            return;
        }

        int chave_apoio = (*tabela1)[p].chave;
        int valor_apoio = (*tabela1)[p].valor;
        (*tabela1)[p].chave = chave;
        (*tabela1)[p].valor = valor;
        chave = chave_apoio;
        valor = valor_apoio;
        contador++;

        p = h2(chave, *n);
        if((*tabela2)[p].chave == -1 || (*tabela2)[p].chave == -2){
            (*tabela2)[p].chave = chave;
            (*tabela2)[p].valor = valor;
            (*ocupacao2)++;
            verificar_tamanho(n, ocupacao1, ocupacao2, tabela1, tabela2);
            return;
        }

        chave_apoio = (*tabela2)[p].chave;
        valor_apoio = (*tabela2)[p].valor;
        (*tabela2)[p].chave = chave;
        (*tabela2)[p].valor = valor;
        chave = chave_apoio;
        valor = valor_apoio;
        contador++;
    }
    redimensionar(tabela1, tabela2, n, ocupacao1, ocupacao2);
    goto inicio_geral;
}


void redimensionar(par** tabela1, par** tabela2, int* n, int* ocupacao1, int* ocupacao2){
    par* antiga1 = *tabela1;
    par* antiga2 = *tabela2;
    int n_antigo = *n;
    *n = 2*n_antigo;
    // Usa os mesmos apontadores para par ("tabelas") já definidos antes para redimensionar para o dobro do tamanho
    *tabela1 = criar_tabela(*n);
    *tabela2 = criar_tabela(*n);
    *ocupacao1 = 0;
    *ocupacao2 = 0;
    for(int i = 0; i<n_antigo; i++){
        if(antiga1[i].chave != -1 && antiga1[i].chave != -2){
            inserir_chave(tabela1, tabela2, antiga1[i].chave, antiga1[i].valor, 15, ocupacao1, ocupacao2, n);
        }
        if(antiga2[i].chave != -1 && antiga2[i].chave != -2){
            inserir_chave(tabela1, tabela2, antiga2[i].chave, antiga2[i].valor, 15, ocupacao1, ocupacao2, n);
        }
    }
    df_free(antiga1);
    df_free(antiga2);
    return;
}

int buscar_chave(par* tabela1, par* tabela2, unsigned int chave, int n, int* valor){
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

void remover_chave(par* tabela1, par* tabela2, unsigned int chave, int n, int* ocupacao1, int* ocupacao2){
    // Remove o par com a chave dada da tabela
    // Como na função de busca apenas a chave é verificada, não
    // é necessário alterar o valor ligado a ela.
    if(tabela1[h1(chave, n)].chave == chave){
        tabela1[h1(chave, n)].chave = -2;
        (*ocupacao1)--;
        return;
    }
    else if (tabela2[h2(chave, n)].chave == chave){
        tabela2[h2(chave, n)].chave = -2;
        (*ocupacao2)--;
        return;
    }
}

void imprimir_linha(par* tabela1, par* tabela2, unsigned int chave, int n){
    int valor;
    if(!buscar_chave(tabela1, tabela2, chave, n, &valor)) printf("%u -> !E\n", chave);
    else printf("%u -> %d\n", chave, valor);
}

int main(void){
    unsigned int chave=0;
    int valor=0;
    char caractere;
    int ocupacao1 = 0, ocupacao2 = 0;
    int LIMITE = 15;
    int n=0;
    par *tabela1 = NULL, *tabela2 = NULL;
    while(scanf(" %c", &caractere)==1 && caractere!='f'){
        if(caractere=='c'){
            df_free(tabela1);
            df_free(tabela2);
            scanf("%d", &n);
            tabela1 = criar_tabela(n);
            tabela2 = criar_tabela(n);
            ocupacao1 = 0;
            ocupacao2 = 0;
        }
        else if(caractere=='i'){
            scanf("%u %d", &chave, &valor);
            inserir_chave(&tabela1, &tabela2, chave, valor, LIMITE, &ocupacao1, &ocupacao2, &n);
        }
        else if(caractere=='r'){
            scanf("%u", &chave);
            remover_chave(tabela1, tabela2, chave, n, &ocupacao1, &ocupacao2);
        }
        else if(caractere=='b'){
            scanf("%u", &chave);
            imprimir_linha(tabela1, tabela2, chave, n);
        }
    }
    df_free(tabela1);
    df_free(tabela2);
    return 0;
}
