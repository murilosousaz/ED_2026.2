#ifndef FILA_H
#define FILA_H

/* TAD Fila de espera FIFO encadeada, com início e fim. */

typedef struct NoEspera {
    int matricula;
    char nome[100];
    struct NoEspera *prox;
} NoEspera;

typedef struct Fila {
    NoEspera *inicio;
    NoEspera *fim;
} Fila;

/* Inicializa a fila vazia. */
void fila_init(Fila *f);

/* Enfileira ao fim. Retorna 1 em sucesso, 0 em falha. */
int fila_enfileirar(Fila *f, int matricula, const char *nome);

/* Desenfileira o primeiro. Copia matrícula e nome se os ponteiros não forem
 * NULL. Retorna 1 em sucesso, 0 se a fila estiver vazia. */
int fila_desenfileirar(Fila *f, int *matricula, char *nome);

/* Consulta a matrícula do primeiro, sem remover. Retorna 1 se existir. */
int fila_primeiro(const Fila *f, int *matricula);

/* Retorna 1 se a matrícula estiver na fila. */
int fila_contem(const Fila *f, int matricula);

/* Cancela a reserva da matrícula em qualquer posição, preservando a ordem
 * relativa dos demais. Retorna 1 se removeu, 0 se não encontrou. */
int fila_remover(Fila *f, int matricula);

/* Libera todos os nós. */
void fila_destruir(Fila *f);

#endif
