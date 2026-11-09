#include <stdlib.h>
#include <string.h>

#include "lista.h"

void lista_init(Lista *l) {
    l->inicio = NULL;
    l->fim = NULL;
}

int lista_inserir_fim(Lista *l, int matricula, const char *nome) {
    NoInscrito *novo = malloc(sizeof(NoInscrito));
    if (novo == NULL) return 0;

    novo->matricula = matricula;
    strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    novo->prox = NULL;

    if (l->fim != NULL) {
        l->fim->prox = novo;
    } else {
        l->inicio = novo;
    }
    l->fim = novo;
    return 1;
}

int lista_contem(const Lista *l, int matricula) {
    for (const NoInscrito *p = l->inicio; p != NULL; p = p->prox) {
        if (p->matricula == matricula) return 1;
    }
    return 0;
}

int lista_remover(Lista *l, int matricula) {
    NoInscrito *ant = NULL;
    NoInscrito *p = l->inicio;

    while (p != NULL && p->matricula != matricula) {
        ant = p;
        p = p->prox;
    }
    if (p == NULL) return 0;

    if (ant != NULL) {
        ant->prox = p->prox;
    } else {
        l->inicio = p->prox;
    }
    if (p == l->fim) {
        l->fim = ant; /* fim passa a ser o anterior (ou NULL se a lista esvaziou) */
    }
    free(p);
    return 1;
}

void lista_destruir(Lista *l) {
    NoInscrito *p = l->inicio;
    while (p != NULL) {
        NoInscrito *t = p;
        p = p->prox;
        free(t);
    }
    l->inicio = NULL;
    l->fim = NULL;
}
