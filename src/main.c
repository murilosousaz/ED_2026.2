#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "abb.h"
#include "atividade.h"
#include "lista.h"
#include "fila.h"
#include "historico.h"

#define MAX_LINHA 4096
#define MAX_TOKENS 8

/* Converte s em inteiro estrito: dígitos decimais apenas, sem lixo após o número. */
static int parse_int(const char *s, long *out) {
    if (s == NULL || *s == '\0') return 0;
    char *end = NULL;
    errno = 0;
    long v = strtol(s, &end, 10);
    if (errno != 0 || end == s || *end != '\0') return 0;
    *out = v;
    return 1;
}

/* Inteiro positivo e dentro do alcance de int. */
static int parse_positivo(const char *s, int *out) {
    long v;
    if (!parse_int(s, &v) || v <= 0 || v > INT_MAX) return 0;
    *out = (int)v;
    return 1;
}

static void imprimir_atividade(Atividade *a) {
    printf("ATIVIDADE %d %s %d %d %d\n",
           a->codigo, a->titulo, a->capacidade, a->qtdInscritos, a->qtdEspera);
}

static void imprimir_resumo(Atividade *a) {
    printf("RESUMO %d %d %d %d\n",
           a->codigo, a->capacidade, a->qtdInscritos, a->qtdEspera);
}

int main(void) {
    NoABB *raiz = NULL;
    Historico hist;
    historico_init(&hist);

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';

        char *tok[MAX_TOKENS];
        int n = 0;
        char *t = strtok(linha, " \t");
        while (t != NULL && n < MAX_TOKENS) {
            tok[n++] = t;
            t = strtok(NULL, " \t");
        }
        if (n == 0) continue; /* linha em branco */

        if (strcmp(tok[0], "FIM") == 0) {
            printf("FIM\n");
            break;
        }

        if (strcmp(tok[0], "CRIAR") == 0) {
            int a, c;
            if (n != 4 || !parse_positivo(tok[1], &a) || !parse_positivo(tok[2], &c)) {
                printf("ERRO PARAMETROS\n");
            } else if (abb_buscar(raiz, a) != NULL) {
                printf("ERRO ATIVIDADE_EXISTE\n");
            } else {
                Atividade *atv = atividade_criar(a, c, tok[3]);
                if (atv == NULL || !abb_inserir(&raiz, atv)) {
                    atividade_destruir(atv);
                    printf("ERRO PARAMETROS\n");
                } else {
                    historico_registrar(&hist, "CADASTRO", a, 0);
                    printf("OK CRIAR %d\n", a);
                }
            }
        } else if (strcmp(tok[0], "INSCRIVER") == 0) {
            int a, m;
            if (n != 4 || !parse_positivo(tok[1], &a) || !parse_positivo(tok[2], &m)) {
                printf("ERRO PARAMETROS\n");
            } else {
                Atividade *atv = abb_buscar(raiz, a);
                if (atv == NULL) {
                    printf("ERRO ATIVIDADE_INEXISTENTE\n");
                } else if (lista_contem(&atv->inscritos, m) ||
                           fila_contem(&atv->espera, m)) {
                    printf("ERRO ALUNO_DUPLICADO\n");
                } else if (atv->qtdInscritos < atv->capacidade) {
                    lista_inserir_fim(&atv->inscritos, m, tok[3]);
                    atv->qtdInscritos++;
                    historico_registrar(&hist, "INSCRICAO", a, m);
                    printf("OK INSCRITO %d %d\n", a, m);
                } else {
                    fila_enfileirar(&atv->espera, m, tok[3]);
                    atv->qtdEspera++;
                    historico_registrar(&hist, "ENTRADA_ESPERA", a, m);
                    printf("OK ESPERA %d %d %d\n", a, m, atv->qtdEspera);
                }
            }
        } else if (strcmp(tok[0], "CANCELAR") == 0) {
            int a, m;
            if (n != 3 || !parse_positivo(tok[1], &a) || !parse_positivo(tok[2], &m)) {
                printf("ERRO PARAMETROS\n");
            } else {
                Atividade *atv = abb_buscar(raiz, a);
                if (atv == NULL) {
                    printf("ERRO ATIVIDADE_INEXISTENTE\n");
                } else if (lista_contem(&atv->inscritos, m)) {
                    lista_remover(&atv->inscritos, m);
                    atv->qtdInscritos--;
                    historico_registrar(&hist, "CANCEL_INSCRICAO", a, m);
                    printf("OK CANCELADO_INSCRITO %d %d\n", a, m);

                    int m2;
                    char nome2[100];
                    if (fila_desenfileirar(&atv->espera, &m2, nome2)) {
                        lista_inserir_fim(&atv->inscritos, m2, nome2);
                        atv->qtdInscritos++;
                        atv->qtdEspera--;
                        historico_registrar(&hist, "PROMOCAO", a, m2);
                        printf("PROMOVIDO %d %d\n", a, m2);
                    }
                } else if (fila_contem(&atv->espera, m)) {
                    fila_remover(&atv->espera, m);
                    atv->qtdEspera--;
                    historico_registrar(&hist, "CANCEL_ESPERA", a, m);
                    printf("OK CANCELADO_ESPERA %d %d\n", a, m);
                } else {
                    printf("ERRO ALUNO_NAO_ENCONTRADO\n");
                }
            }
        } else if (strcmp(tok[0], "AUMENTAR") == 0) {
            int a, d;
            if (n != 3 || !parse_positivo(tok[1], &a) || !parse_positivo(tok[2], &d)) {
                printf("ERRO PARAMETROS\n");
            } else {
                Atividade *atv = abb_buscar(raiz, a);
                if (atv == NULL) {
                    printf("ERRO ATIVIDADE_INEXISTENTE\n");
                } else {
                    atv->capacidade += d;
                    historico_registrar(&hist, "AUMENTO_CAPACIDADE", a, 0);

                    int *prom = atv->qtdEspera > 0
                                ? malloc(sizeof(int) * (size_t)atv->qtdEspera)
                                : NULL;
                    int k = 0;
                    while (atv->qtdInscritos < atv->capacidade) {
                        int m2;
                        char nome2[100];
                        if (!fila_desenfileirar(&atv->espera, &m2, nome2)) break;
                        lista_inserir_fim(&atv->inscritos, m2, nome2);
                        atv->qtdInscritos++;
                        atv->qtdEspera--;
                        historico_registrar(&hist, "PROMOCAO", a, m2);
                        prom[k] = m2;
                        k++;
                    }
                    printf("OK AUMENTAR %d %d %d\n", a, atv->capacidade, k);
                    for (int i = 0; i < k; i++) {
                        printf("PROMOVIDO %d %d\n", a, prom[i]);
                    }
                    free(prom);
                }
            }
        } else if (strcmp(tok[0], "BUSCAR") == 0) {
            int a;
            if (n != 2 || !parse_positivo(tok[1], &a)) {
                printf("ERRO PARAMETROS\n");
            } else {
                Atividade *atv = abb_buscar(raiz, a);
                if (atv == NULL) {
                    printf("ERRO ATIVIDADE_INEXISTENTE\n");
                } else {
                    imprimir_atividade(atv);
                }
            }
        } else if (strcmp(tok[0], "INSCRITOS") == 0) {
            int a;
            if (n != 2 || !parse_positivo(tok[1], &a)) {
                printf("ERRO PARAMETROS\n");
            } else {
                Atividade *atv = abb_buscar(raiz, a);
                if (atv == NULL) {
                    printf("ERRO ATIVIDADE_INEXISTENTE\n");
                } else {
                    printf("INSCRITOS %d %d", a, atv->qtdInscritos);
                    for (NoInscrito *p = atv->inscritos.inicio; p != NULL; p = p->prox) {
                        printf(" %d", p->matricula);
                    }
                    printf("\n");
                }
            }
        } else if (strcmp(tok[0], "ESPERA") == 0) {
            int a;
            if (n != 2 || !parse_positivo(tok[1], &a)) {
                printf("ERRO PARAMETROS\n");
            } else {
                Atividade *atv = abb_buscar(raiz, a);
                if (atv == NULL) {
                    printf("ERRO ATIVIDADE_INEXISTENTE\n");
                } else {
                    printf("ESPERA %d %d", a, atv->qtdEspera);
                    for (NoEspera *p = atv->espera.inicio; p != NULL; p = p->prox) {
                        printf(" %d", p->matricula);
                    }
                    printf("\n");
                }
            }
        } else if (strcmp(tok[0], "LISTAR") == 0) {
            if (n != 1) {
                printf("ERRO PARAMETROS\n");
            } else {
                printf("LISTAR %d\n", abb_contar(raiz));
                abb_em_ordem(raiz, imprimir_atividade);
            }
        } else if (strcmp(tok[0], "ARVORE") == 0) {
            if (n != 1) {
                printf("ERRO PARAMETROS\n");
            } else {
                printf("ARVORE %d %d\n", abb_contar(raiz), abb_altura(raiz));
            }
        } else if (strcmp(tok[0], "HISTORICO") == 0) {
            if (n != 1) {
                printf("ERRO PARAMETROS\n");
            } else {
                int total = 0;
                for (Evento *e = hist.inicio; e != NULL; e = e->prox) total++;
                printf("HISTORICO %d\n", total);
                for (Evento *e = hist.inicio; e != NULL; e = e->prox) {
                    printf("EVENTO %d %s %d %d\n",
                           e->seq, e->tipo, e->codigo, e->matricula);
                }
            }
        } else if (strcmp(tok[0], "RELATORIO") == 0) {
            if (n != 1) {
                printf("ERRO PARAMETROS\n");
            } else {
                printf("RELATORIO %d\n", abb_contar(raiz));
                abb_em_ordem(raiz, imprimir_resumo);
            }
        } else {
            printf("ERRO PARAMETROS\n");
        }
    }

    abb_destruir(raiz);
    historico_destruir(&hist);
    return 0;
}
