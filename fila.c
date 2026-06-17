#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria() {
    struct fila_t *f = malloc(sizeof(struct fila_t));
    if (!f) return NULL;

    // Inicialização segura: uma fila recém-criada não tem nenhum nó
    f->prim = NULL;
    f->ult = NULL;
    f->num = 0;
    return f;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi(struct fila_t *f) {
    if (!f) return NULL;

    struct fila_nodo_t *atual = f->prim;
    // Precisamos percorrer a fila nodo a nodo
    while (atual) {
        // Importante: guardamos o próximo antes de liberar o atual
        struct fila_nodo_t *prox = atual->prox;
        free(atual); // Libera o nodo
        atual = prox;
    }
    free(f); // Libera a estrutura da fila
    return NULL;
}

// Insere um item no final da fila (politica FIFO).
// Retorno: 1 se tiver sucesso ou 0 se falhar.
int fila_insere(struct fila_t *f, int item) {
    if (!f) return 0;
    struct fila_nodo_t *novo = malloc(sizeof(struct fila_nodo_t));
    if (!novo) return 0; // Verifica se há memória disponível
    
    novo->item = item;
    novo->prox = NULL; // É o novo último, logo não tem ninguém depois dele
    
    // Caso especial: fila vazia
    if (f->num == 0) {
        f->prim = f->ult = novo;
    } else {
        // Caso geral: liga o antigo último ao novo nodo
        f->ult->prox = novo; // O antigo último aponta para o novo
        f->ult = novo;       // O novo vira o último
    }
    f->num++; 
    return 1;
}

// Retira o primeiro item da fila e o devolve
// Retorno 1 se a operação foi bem sucedida e 0 caso contrário
int fila_retira(struct fila_t *f, int *item) {
    // Se a fila estiver vazia ou for inexistente, não há o que retirar
    if (!f || f->num == 0) return 0;
    
    struct fila_nodo_t *aux = f->prim;
    *item = aux->item; // Copiamos o dado para a variável externa
    f->prim = aux->prox; // Movemos o início para o próximo nodo

    // Caso crítico: se após a remoção a fila ficou vazia (prim virou NULL),
    // o ponteiro 'ult' também deve ser NULL para manter a integridade.
    if (f->prim == NULL) f->ult = NULL; // Se ficou vazia, ajusta o ult
    
    free(aux); // Removemos o nodo da memória
    f->num--;
    return 1;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho(struct fila_t *f) {
    if (f == NULL) {
        return -1; // Retorna -1 se houver erro (fila não existe)
    }
    return f->num; // Retorna o tamanho correto caso contrário
}

// Imprime no formato: (item) (item) ...
void fila_imprime(struct fila_t *f) {
    if (!f) return;
    struct fila_nodo_t *atual = f->prim;
    while (atual) {
        // Percorremos apenas seguindo os ponteiros 'prox'
        printf("(%d) ", atual->item);
        atual = atual->prox;
    }
}