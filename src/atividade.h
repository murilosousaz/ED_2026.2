#ifndef ATIVIDADE_H
#define ATIVIDADE_H

#include "lista.h"
#include "fila.h"

/* TAD Atividade: código único, título, capacidade e contadores, além da lista
 * de inscritos e da fila de espera. */

typedef struct Atividade {
    int codigo;
    char titulo[100];
    int capacidade;
    int qtdInscritos;
    int qtdEspera;
    Lista inscritos;
    Fila espera;
} Atividade;

/* Cria uma atividade com lista e fila vazias. Retorna NULL em falha de alocação. */
Atividade *atividade_criar(int codigo, int capacidade, const char *titulo);

/* Libera a atividade e suas estruturas. */
void atividade_destruir(Atividade *a);

#endif
