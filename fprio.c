/* ESSE TAD GERÊNCIARÁ A SEGUENCIA DOS EVENTOS

ele guarda todos os eventos futuros

fila de prioridades força q o evento com o menor tmepo esteja sempre na frente da fila

*/
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria()
{
    // aloca a estrutura da fila
    struct fprio_t *f = malloc(sizeof(struct fprio_t));

    if (!f)
        return NULL; // se o sistema não tiver memória, null

    // Inicializa os ponteiros vazios
    f->prim = NULL;
    f->fim = NULL;
    f->num = 0;

    return f;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi(struct fprio_t *f)
{
    if (!f)
        return NULL;

    struct fpnodo_t *atual = f->prim;
    // Percorre todos os nodos da lista
    while (atual)
    {
        struct fpnodo_t *aux = atual->prox; // guarda qual é o próximo antes de apagar
        if (atual->item != NULL)
            free(atual->item); //*******************
        free(atual);           // libera o nodo atual
        atual = aux;           // avança na lista
    }

    // Libera a estrutura da fila em si
    free(f);
    return NULL;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere(struct fprio_t *f, void *item, int tipo, int prio)
{
    // 1-validação de segurança
    if (!f || !item)
        return -1;

    // 2-verificação de item duplicado
    struct fpnodo_t *atual = f->prim;
    while (atual)
    {
        if (atual->item == item)
            return -1; // Erro: ele já existe na fila
        atual = atual->prox;
    }

    // 3 - Criação do novo nodo
    struct fpnodo_t *novo = malloc(sizeof(struct fpnodo_t));
    if (!novo)
        return -1;

    // preenche os dados do evento no nodo
    novo->item = item;
    novo->tipo = tipo;
    novo->prio = prio;
    novo->prox = NULL; // ainda não aponta para ngm

    // 4- Busca o lugar certo
    // Vamos percorrer a fila procurando onde o novo elemento deve entrar
    atual = f->prim;
    struct fpnodo_t *ant = NULL;

    // Enquanto não chegarmos ao fim e a prioridade do atual for menor ou igual
    // a do novo, continuamos avançando.
    while (atual != NULL && atual->prio <= prio)
    {
        ant = atual;
        atual = atual->prox;
    }

    // 5 - Costurar
    if (ant == NULL)
    {
        novo->prox = f->prim; // insere no inicio da fila
        f->prim = novo;
        // se a fila estava vazia, o novo tbm é o fim
        if (f->fim == NULL)
            f->fim = novo;
    }
    else
    {
        // insere no meio ou no fim
        novo->prox = atual;
        ant->prox = novo;
        // se o atual é NULL, means que o novo é o novo fim da fila
        if (atual == NULL)
            f->fim = novo;
    }

    // 6- Incrementa e retorna
    f->num++;
    return f->num; // retorna o num de itens na fila
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira(struct fprio_t *f, int *tipo, int *prio)
{
    // 1. Validação: se a fila não existir ou estiver vazia, não há o que retirar
    if (!f || f->prim == NULL || f->num == 0)
        return NULL;

    // 2. Guarda o nodo que está no início (f->prim)
    // Este é o evento que acontece mais cedo no tempo
    struct fpnodo_t *aux = f->prim; // aux segura o primeiro nodo

    // 3. Extrai as informações que o simulador precisa
    void *item_retirado = aux->item;
    *tipo = aux->tipo;
    *prio = aux->prio;

    // 4. Faz o 'prim' apontar para o próximo elemento da fila
    f->prim = aux->prox;

    /// 5. Ajuste de segurança: se a fila ficou vazia (o prim virou NULL),
    // o fim também precisa ser NULL.
    if (f->prim == NULL)
    {
        f->fim = NULL;
    }

    // 6. Libera a memória do nodo (mas NÃO do 'item', pois ele é o evento!)
    free(aux);

    // 7. Atualiza o contador e devolve o item para o 'main' processar
    f->num--;
    return item_retirado;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho(struct fprio_t *f)
{
    if (!f || f->num < 0)
        return -1;
    return f->num;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime(struct fprio_t *f)
{
    if (!f || f->num == 0) return; // Se a fila não existe ou está vazia, não faz nada

    struct fpnodo_t *atual = f->prim; // Começa pelo primeiro nodo

    while (atual != NULL)
    {
        // Imprime o tipo e a prioridade
        printf("(%d %d)", atual->tipo, atual->prio);
        
        // Se existir um próximo nodo, imprime um espaço para separar
        if (atual->prox != NULL) {
            printf(" ");
        }
        
        // Avança para o próximo nodo
        atual = atual->prox;
    }
}
