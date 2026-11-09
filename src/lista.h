#ifndef LISTA_H
#define LISTA_H

/* TAD Lista simplesmente encadeada de inscritos, com início e fim. */

typedef struct NoInscrito {
    int matricula;
    char nome[100];
    struct NoInscrito *prox;
} NoInscrito;

typedef struct Lista {
    NoInscrito *inicio;
    NoInscrito *fim;
} Lista;

/* Inicializa a lista vazia. */
void lista_init(Lista *l);

/* Acrescenta um estudante ao fim da lista. Retorna 1 em sucesso, 0 em falha. */
int lista_inserir_fim(Lista *l, int matricula, const char *nome);

/* Retorna 1 se a matrícula estiver na lista, 0 caso contrário. */
int lista_contem(const Lista *l, int matricula);

/* Remove o nó da matrícula em qualquer posição (início, meio ou fim).
 * Retorna 1 se removeu, 0 se não encontrou. O ponteiro fim nunca fica
 * apontando para memória liberada. */
int lista_remover(Lista *l, int matricula);

/* Libera todos os nós. */
void lista_destruir(Lista *l);

#endif
