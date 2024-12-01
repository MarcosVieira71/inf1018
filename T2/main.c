#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cria_func.h"

// Função de soma com três parâmetros
int soma(int x, int y, int z) {
    return x + y + z;
}

// Função de subtração com um parâmetro amarrado
int subtrai(int x, int y) {
    return x - y;
}

// Função para calcular fatorial (sem parâmetros)
int fatorial(int x) {
    int result = 1;
    for (int i = 1; i <= x; i++) {
        result *= i;
    }
    return result;
}

// Função de concatenação de strings
char* concatena(char* s1, char* s2) {
    char* result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

// Função de comparação entre dois ponteiros de inteiros
int compare(int* x, int* y) {
    return *x - *y;
}

typedef int (*func_ptr_3)(int, int, int);
typedef int (*func_ptr_2)(int, int);
typedef int (*func_ptr_1)(int);
typedef char* (*func_ptr_2str)(char*);
typedef int (*func_ptr_2ptr)(int*);

int main(void) {
    DescParam params[3];
    unsigned char codigo[500];
    func_ptr_3 f_soma;
    func_ptr_2 f_subtrai;
    func_ptr_1 f_fatorial;
    func_ptr_2str f_concatena;
    func_ptr_2ptr f_compare;
    int i, result;
    int a = 10, b = 5;
    char str1[] = "Hello, ";
    char str2[] = "World!";

    // Teste 1: soma com parâmetro fixo
    params[0].tipo_val = INT_PAR;
    params[0].orig_val = PARAM;
    params[1].tipo_val = INT_PAR;
    params[1].orig_val = PARAM;
    params[2].tipo_val = INT_PAR;
    params[2].orig_val = FIX;
    params[2].valor.v_int = 5;

    cria_func(soma, params, 3, codigo);
    f_soma = (func_ptr_3) codigo;
    printf("Soma: %d\n", f_soma(3, 4, 0)); // Esperado: 3 + 4 + 5 = 12

    // Teste 2: subtração com primeiro parâmetro amarrado
    params[0].tipo_val = INT_PAR;
    params[0].orig_val = IND;
    params[0].valor.v_ptr = &a;
    params[1].tipo_val = INT_PAR;
    params[1].orig_val = PARAM;

    cria_func(subtrai, params, 2, codigo);
    f_subtrai = (func_ptr_2) codigo;
    printf("Subtração: %d\n", f_subtrai(15, 0)); // Esperado: 10 - 15 = -5

    // Teste 3: fatorial com parâmetro fixo
    params[0].tipo_val = INT_PAR;
    params[0].orig_val = FIX;
    params[0].valor.v_int = 5;

    cria_func(fatorial, params, 1, codigo);
    f_fatorial = (func_ptr_1) codigo;
    printf("Fatorial: %d\n", f_fatorial(0)); // Esperado: 5! = 120

    // Teste 4: concatenação de strings com o primeiro parâmetro fixo
    params[0].tipo_val = PTR_PAR;
    params[0].orig_val = FIX;
    params[0].valor.v_ptr = str1;
    params[1].tipo_val = PTR_PAR;
    params[1].orig_val = PARAM;

    cria_func(concatena, params, 2, codigo);
    f_concatena = (func_ptr_2str) codigo;
    char* resultado_concat = f_concatena(str2);
    printf("Concatenação: %s\n", resultado_concat); // Esperado: "Hello, World!"


    return 0;
}





