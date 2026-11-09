#include <stdlib.h>
#include <string.h>

#include "fila.h"

void fila_init(Fila *f) {
    f->inicio = NULL;
    f->fim = NULL;
}

int fila_enfileirar(Fila *f, int matricula, const char *nome) {
    NoEspera *novo = malloc(sizeof(NoEspera));
    if (novo == NULL) return 0;

    novo->matricula = matricula;
    strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    novo->prox = NULL;

    if (f->fim != NULL) {
        f->fim->prox = novo;
    } else {
        f->inicio = novo;
    }
    f->fim = novo;
    return 1;
}

int fila_desenfileirar(Fila *f, int *matricula, char *nome) {
    if (f->inicio == NULL) return 0;

    NoEspera *t = f->inicio;
    f->inicio = t->prox;
    if (f->inicio == NULL) {
        f->fim = NULL;
    }

    if (matricula != NULL) *matricula = t->matricula;
    if (nome != NULL) {
        strncpy(nome, t->nome, 99);
        nome[99] = '\0';
    }
    free(t);
    return 1;
}

int fila_primeiro(const Fila *f, int *matricula) {
    if (f->inicio == NULL) return 0;
    if (matricula != NULL) *matricula = f->inicio->matricula;
    return 1;
}

int fila_contem(const Fila *f, int matricula) {
    for (const NoEspera *p = f->inicio; p != NULL; p = p->prox) {
        if (p->matricula == matricula) return 1;
    }
    return 0;
}

int fila_remover(Fila *f, int matricula) {
    NoEspera *ant = NULL;
    NoEspera *p = f->inicio;

    while (p != NULL && p->matricula != matricula) {
        ant = p;
        p = p->prox;
    }
    if (p == NULL) return 0;

    if (ant != NULL) {
        ant->prox = p->prox;
    } else {
        f->inicio = p->prox;
    }
    if (p == f->fim) {
        f->fim = ant;
    }
    free(p);
    return 1;
}

void fila_destruir(Fila *f) {
    NoEspera *p = f->inicio;
    while (p != NULL) {
        NoEspera *t = p;
        p = p->prox;
        free(t);
    }
    f->inicio = NULL;
    f->fim = NULL;
}
