#ifndef ABB_H
#define ABB_H

#include "atividade.h"

/* TAD Árvore Binária de Busca indexada pelo código da atividade. */

typedef struct NoABB {
    Atividade *atividade;
    struct NoABB *esq;
    struct NoABB *dir;
} NoABB;

/* Insere a atividade na árvore. Não aceita código duplicado.
 * Retorna 1 se inseriu, 0 se o código já existia ou houve falha de alocação. */
int abb_inserir(NoABB **raiz, Atividade *atividade);

/* Retorna a atividade com o código, ou NULL se não existir. */
Atividade *abb_buscar(NoABB *raiz, int codigo);

/* Percorre em ordem crescente, chamando visita(a) para cada atividade. */
void abb_em_ordem(NoABB *raiz, void (*visita)(Atividade *a));

/* Quantidade de nós. */
int abb_contar(const NoABB *raiz);

/* Altura: -1 para vazia, 0 para só raiz. */
int abb_altura(const NoABB *raiz);

/* Libera a árvore inteira, incluindo as atividades. */
void abb_destruir(NoABB *raiz);

#endif
