#include <stdio.h>
#include <stdlib.h>

int main(){
    declaracaoArray();
    inicializacaoArray();
    enderecamentoArray();
    arrayPonteiro();
    return 0;
}

int declaracaoArray(){
    int v[5];
    float notas[10];
    char nome[50];
}

int inicializacaoArray(){
    int v1[5] = {1, 2, 3, 4, 5};
    int v2[] = {10, 20, 30};
}

int enderecamentoArray(){
    int v[3] = {10, 20, 30};
    printf("&v[0] = %p\n", &v[0]);
    printf("&v[1] = %p\n", &v[1]);
    printf("&v[2] = %p\n", &v[2]);
}

int arrayPonteiro(){
    int v[10];
    int *p = v;
    printf("&zu\n", sizeof(v));
    printf("&zu\n", sizeof(p));
}

int bufferOverflow(){
    int v[3];
    v[10] = 99;
}

int stackSmashing(){
    char nome[8];
    gets(nome);
}

