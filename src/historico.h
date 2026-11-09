#ifndef HISTORICO_H
#define HISTORICO_H

/* TAD Histórico global: lista encadeada (início e fim) de eventos bem-sucedidos
 * em ordem cronológica. Cada evento guarda cópia dos valores, não ponteiros. */

typedef struct Evento {
    int seq;
    char tipo[32];
    int codigo;
    int matricula;
    struct Evento *prox;
} Evento;

typedef struct Historico {
    Evento *inicio;
    Evento *fim;
} Historico;

/* Inicializa o histórico vazio. */
void historico_init(Historico *h);

/* Registra um evento no fim, com sequência crescente a partir de 1.
 * Retorna 1 em sucesso, 0 em falha de alocação. */
int historico_registrar(Historico *h, const char *tipo, int codigo, int matricula);

/* Libera todos os eventos. */
void historico_destruir(Historico *h);

#endif
