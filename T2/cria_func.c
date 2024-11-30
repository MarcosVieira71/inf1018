/* Julia Gomes Zibordi 2320934 3WA */
/* Marcos Paulo Marinho Vieira 2320466 3WA */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "cria_func.h"

int instrucoesFinaisDaPilha(void *f, unsigned char* codigo, int* idxCodigo);
int instrucoesIniciasDaPilha(unsigned char* codigo, int* idxCodigo);
void cria_func(void* f, DescParam params[], int n, unsigned char *codigo);

int instrucoesIniciasDaPilha(unsigned char* codigo, int* idxCodigo){
    codigo[(*idxCodigo)++] = 0x55;    //pushq %rbp
    codigo[(*idxCodigo)++] = 0x48;    //movq %rsp, %rbp
    codigo[(*idxCodigo)++] = 0x89; 
    codigo[(*idxCodigo)++] = 0xe5; 
    printf("idxCodigo %d\n", *idxCodigo);
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
    printf("idxCodigo %d\n", *idxCodigo);

    return 0;
}

int guardarInformacoesPilha(unsigned char* codigo, int* idxCodigo){
    //subq $-32, %rsp
    codigo[(*idxCodigo)++] = 0x48;
    codigo[(*idxCodigo)++] = 0x83;
    codigo[(*idxCodigo)++] = 0xec;
    codigo[(*idxCodigo)++] = 0x20;

    //movq %rdi, -8(%rbp)
    codigo[(*idxCodigo)++] = 0x48;
    codigo[(*idxCodigo)++] = 0x89;
    codigo[(*idxCodigo)++] = 0x7d;
    codigo[(*idxCodigo)++] = 0xf8;

    
    //movq %rsi, -16(%rbp)
    codigo[(*idxCodigo)++] = 0x48;
    codigo[(*idxCodigo)++] = 0x89;
    codigo[(*idxCodigo)++] = 0x75;
    codigo[(*idxCodigo)++] = 0xf0;
    
    //movq %rdx, -24(%rbp)
    codigo[(*idxCodigo)++] = 0x48;
    codigo[(*idxCodigo)++] = 0x89;
    codigo[(*idxCodigo)++] = 0x55;
    codigo[(*idxCodigo)++] = 0xe8;    
    return 0;
}

int passaInformacoesParaArgumentos(unsigned char* codigo, int* idxCodigo, unsigned char registrador, unsigned posPilha){
    codigo[(*idxCodigo)++] = 0x48;
    codigo[(*idxCodigo)++] = 0x8b;
    if(posPilha == 24){
        codigo[(*idxCodigo)++] = 0x7d;
        codigo[(*idxCodigo)++] = 0xe8; 
    }
    else if(posPilha == 16){

        codigo[(*idxCodigo)++] = 0x75;
        codigo[(*idxCodigo)++] = 0xe8; 
    }
    else if(posPilha == 8){

        codigo[(*idxCodigo)++] = 0x55;
        codigo[(*idxCodigo)++] = 0xf8; 
    }
    return 0;
}


void cria_func(void* f, DescParam params[], int n, unsigned char *codigo) {
    if (n < 1 || n > 3) {
        printf("O valor de parâmetros deve ser entre 1 e 3\n");
        exit(1);
    }
    int idxCodigo = 0;
    int instrucoesIniciais = instrucoesIniciasDaPilha(codigo, &idxCodigo);
    for(int i = 0; i < n; i++){
        if(params[i].tipo_val == INT_PAR){
            if(params[i].orig_val == PARAM){
                unsigned char registradores[] = {};
                unsigned char endPilha[] = {}; 

            }
            else if (params[i].orig_val == FIX){
                unsigned char registradores[] = {0xbf, 0xbe, 0xba};
                codigo[idxCodigo++] = registradores[i];
                int inteiroFixo = params[i].valor.v_int;
                for (int i = 0; i < sizeof(int); i++) {
                    unsigned char byteAtual = inteiroFixo & 0xFF;
                    codigo[idxCodigo++] = byteAtual;                 
                    inteiroFixo >>= 8;                           
                }
            }
            else{
                unsigned char registradores[] = {0x3b, 0x33, 0x13};
                codigo[idxCodigo++] = 0x48;
                codigo[idxCodigo++] = 0xbb;
                long int valorIntPonteiroInd = (long int) params[i].valor.v_ptr;
                for (int i = 0; i < sizeof(int*); i++) {
                    unsigned char byteAtual = valorIntPonteiroInd & 0xFF;
                    codigo[idxCodigo++] = byteAtual;    
                    valorIntPonteiroInd >>= 8;                           
                }                
                codigo[idxCodigo++] = 0x8b;
                codigo[idxCodigo++] = registradores[i];
            }
        } 
        else if(params[i].tipo_val == PTR_PAR){
            if(params[i].orig_val == PARAM){


            }
            else if (params[i].orig_val == FIX){
                unsigned char registradores[] = {0xbf, 0xbe, 0xba};
                codigo[idxCodigo++] = 0x48;
                codigo[idxCodigo++] = registradores[i];
                long int valorPonteiroFixo = (long int) params[i].valor.v_ptr;
                for (int i = 0; i < sizeof(int*); i++) {
                    unsigned char byteAtual = valorPonteiroFixo & 0xFF;
                    codigo[idxCodigo++] = byteAtual;                 
                    valorPonteiroFixo >>= 8;                           
                }
            }
            else{
                unsigned char registradores[] = {0x3b, 0x33, 0x13};
                codigo[idxCodigo++] = 0x48;
                codigo[idxCodigo++] = 0xbb;
                long int valorPonteiroInd = (long int) params[i].valor.v_ptr;
                for (int i = 0; i < sizeof(int*); i++) {
                    unsigned char byteAtual = valorPonteiroInd & 0xFF;
                    codigo[idxCodigo++] = byteAtual;                 
                    valorPonteiroInd >>= 8;                           
                }                
                codigo[idxCodigo++] = 0x48;
                codigo[idxCodigo++] = 0x8b;
                codigo[idxCodigo++] = registradores[i];
            }
        }
    }

    int instrucoesFinais = instrucoesFinaisDaPilha(f, codigo, &idxCodigo);
    if(instrucoesIniciais || instrucoesFinais) exit(1);
    for(int i = 0; i < idxCodigo; i++){
        printf("%hhx\n", codigo[i]);
    }
}

