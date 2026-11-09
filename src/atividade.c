#include <stdlib.h>
#include <string.h>

#include "atividade.h"

Atividade *atividade_criar(int codigo, int capacidade, const char *titulo) {
    Atividade *a = malloc(sizeof(Atividade));
    if (a == NULL) return NULL;

    a->codigo = codigo;
    a->capacidade = capacidade;
    a->qtdInscritos = 0;
    a->qtdEspera = 0;
    strncpy(a->titulo, titulo, sizeof(a->titulo) - 1);
    a->titulo[sizeof(a->titulo) - 1] = '\0';

    lista_init(&a->inscritos);
    fila_init(&a->espera);
    return a;
}

void atividade_destruir(Atividade *a) {
    if (a == NULL) return;
    lista_destruir(&a->inscritos);
    fila_destruir(&a->espera);
    free(a);
}
