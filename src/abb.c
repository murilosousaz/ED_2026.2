#include <stdlib.h>

#include "abb.h"

int abb_inserir(NoABB **raiz, Atividade *atividade) {
    if (*raiz == NULL) {
        NoABB *novo = malloc(sizeof(NoABB));
        if (novo == NULL) return 0;
        novo->atividade = atividade;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
        return 1;
    }

    NoABB *n = *raiz;
    if (atividade->codigo < n->atividade->codigo) {
        return abb_inserir(&n->esq, atividade);
    }
    if (atividade->codigo > n->atividade->codigo) {
        return abb_inserir(&n->dir, atividade);
    }
    return 0; /* código duplicado */
}

Atividade *abb_buscar(NoABB *raiz, int codigo) {
    while (raiz != NULL) {
        if (codigo == raiz->atividade->codigo) {
            return raiz->atividade;
        }
        if (codigo < raiz->atividade->codigo) {
            raiz = raiz->esq;
        } else {
            raiz = raiz->dir;
        }
    }
    return NULL;
}

void abb_em_ordem(NoABB *raiz, void (*visita)(Atividade *a)) {
    if (raiz == NULL) return;
    abb_em_ordem(raiz->esq, visita);
    visita(raiz->atividade);
    abb_em_ordem(raiz->dir, visita);
}

int abb_contar(const NoABB *raiz) {
    if (raiz == NULL) return 0;
    return 1 + abb_contar(raiz->esq) + abb_contar(raiz->dir);
}

int abb_altura(const NoABB *raiz) {
    if (raiz == NULL) return -1;
    int he = abb_altura(raiz->esq);
    int hd = abb_altura(raiz->dir);
    return 1 + (he > hd ? he : hd);
}

void abb_destruir(NoABB *raiz) {
    if (raiz == NULL) return;
    abb_destruir(raiz->esq);
    abb_destruir(raiz->dir);
    atividade_destruir(raiz->atividade);
    free(raiz);
}
