#include <stdlib.h>
#include <string.h>

#include "historico.h"

void historico_init(Historico *h) {
    h->inicio = NULL;
    h->fim = NULL;
}

int historico_registrar(Historico *h, const char *tipo, int codigo, int matricula) {
    Evento *e = malloc(sizeof(Evento));
    if (e == NULL) return 0;

    e->seq = (h->fim != NULL) ? h->fim->seq + 1 : 1;
    strncpy(e->tipo, tipo, sizeof(e->tipo) - 1);
    e->tipo[sizeof(e->tipo) - 1] = '\0';
    e->codigo = codigo;
    e->matricula = matricula;
    e->prox = NULL;

    if (h->fim != NULL) {
        h->fim->prox = e;
    } else {
        h->inicio = e;
    }
    h->fim = e;
    return 1;
}

void historico_destruir(Historico *h) {
    Evento *p = h->inicio;
    while (p != NULL) {
        Evento *t = p;
        p = p->prox;
        free(t);
    }
    h->inicio = NULL;
    h->fim = NULL;
}
