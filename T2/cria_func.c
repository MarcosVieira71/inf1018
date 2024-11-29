/* Julia Gomes Zibordi 2320934 3WA */
/* Marcos Paulo Marinho Vieira 2320466 3WA */

#include <stdio.h>
#include <stdint.h>
#include "cria_func.h"

int instrucoesFinaisDaPilha(void *f, unsigned char* codigo, int* idxCodigo);
int instrucoesIniciasDaPilha(unsigned char* codigo, int* idxCodigo);
void cria_func(void* f, DescParam params[], int n, unsigned char *codigo);

int instrucoesIniciasDaPilha(unsigned char* codigo, int* idxCodigo){
    codigo[(*idxCodigo)++] = 0x55;    //pushq %rbp
    codigo[(*idxCodigo)++] = 0x48;    //movq %rsp, %rbp
    codigo[(*idxCodigo)++] = 0x89; 
    codigo[(*idxCodigo)++] = 0xe5; 
    return 0;
}

int instrucoesFinaisDaPilha(void *f, unsigned char* codigo, int* idxCodigo){
    codigo[(*idxCodigo)++] = 0x48; 
    codigo[(*idxCodigo)++] = 0xb8;
    
    unsigned long int enderecoFuncao = (unsigned long int)f; 

    for (int i = 0; i < 8; i++) {
        unsigned char byteAtual = enderecoFuncao & 0xFF;
        codigo[(*idxCodigo)++] = byteAtual;                 
        enderecoFuncao >>= 8;                           
    }

    //chamada da funcao 
    codigo[(*idxCodigo)++] = 0xff; 
    codigo[(*idxCodigo)++] = 0xd0; //call %rax
    codigo[(*idxCodigo)++] = 0xc9; //leave
    codigo[(*idxCodigo)++] = 0xc3; //ret
    return 0;
}

void cria_func(void* f, DescParam params[], int n, unsigned char *codigo) {
    if (n < 1 || n > 3) {
        printf("O valor de parâmetros deve ser entre 1 e 3\n");
        exit(1);
    }
    int idxCodigo = 0;
    int instrucoesIniciais = instrucoesIniciasDaPilha(codigo, &idxCodigo);
    int instrucoesFinais = instrucoesFinaisDaPilha(f, codigo, &idxCodigo);
    for(int i = 0; i < n; i++){

    }

    if(instrucoesIniciais || instrucoesFinais) exit(1); 

}

