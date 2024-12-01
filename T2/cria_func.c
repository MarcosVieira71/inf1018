#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "cria_func.h"

void instrucoesFinaisDaPilha(void *f, unsigned char* codigo, int* idxCodigo);
void instrucoesIniciaisDaPilha(unsigned char* codigo, int* idxCodigo);
void cria_func(void* f, DescParam params[], int n, unsigned char *codigo);
void guardarInformacoesPilha(unsigned char* codigo, int* idxCodigo);

void instrucoesIniciaisDaPilha(unsigned char* codigo, int* idxCodigo){
    codigo[(*idxCodigo)++] = 0x55;    //pushq %rbp
    codigo[(*idxCodigo)++] = 0x48;    //movq %rsp, %rbp
    codigo[(*idxCodigo)++] = 0x89; 
    codigo[(*idxCodigo)++] = 0xe5; 
    printf("idxCodigo %d\n", *idxCodigo);
}

void instrucoesFinaisDaPilha(void *f, unsigned char* codigo, int* idxCodigo){
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
}

void guardarInformacoesPilha(unsigned char* codigo, int* idxCodigo){
    //subq $32, %rsp
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
}

void passaInformacoesParaArgumentos(unsigned char* codigo, int* idxCodigo, unsigned int posPilha, int iFor, int ehInteiro) {
    if(!ehInteiro){
        codigo[(*idxCodigo)++] = 0x48;    
    }
    codigo[(*idxCodigo)++] = 0x8b;    

    unsigned char registradores[] = {0x7d, 0x75, 0x55};
    codigo[(*idxCodigo)++] = registradores[iFor]; 
    
    int posIdx = posPilha/8 - 1; 

    if(posIdx > 2 || posIdx < 0){
        printf("Erro: posição de pilha inválida (%u)\n", posPilha);
        exit(1);
    }

    unsigned char posicaoRbp[] = {0xf8, 0xf0, 0xe8};
    codigo[(*idxCodigo)++] = posicaoRbp[posIdx]; 

}



void cria_func(void* f, DescParam params[], int n, unsigned char *codigo) {
    if (n < 1 || n > 3) {
        printf("Erro: o número de parâmetros deve ser entre 1 e 3\n");
        exit(1);
    }

    int idxCodigo = 0;

    int numParams = 0;
    
    instrucoesIniciaisDaPilha(codigo, &idxCodigo);
    guardarInformacoesPilha(codigo, &idxCodigo);

    unsigned char registradores_int[] = {0xbf, 0xbe, 0xba};
    unsigned char registradores_ptr[] = {0x3f, 0x36, 0x32}; 
    unsigned int endPilha[] = {8, 16, 24};                  

    for (int i = 0; i < n; i++) {
        if (params[i].orig_val == PARAM) {
            int ehInteiro;
            if(params[i].tipo_val == INT_PAR) ehInteiro = 1;
            else ehInteiro = 0;
            passaInformacoesParaArgumentos(codigo, &idxCodigo, endPilha[numParams], i, ehInteiro);
            numParams++;
        }

        else if (params[i].orig_val == FIX) {
            if (params[i].tipo_val == INT_PAR) {
                
                codigo[idxCodigo++] = registradores_int[i]; 
                int valorFixo = params[i].valor.v_int;
                for (int j = 0; j < sizeof(int); j++) {
                    codigo[idxCodigo++] = valorFixo & 0xFF;
                    valorFixo >>= 8;
                }
            } else if (params[i].tipo_val == PTR_PAR) {
                
                codigo[idxCodigo++] = 0x48;               
                codigo[idxCodigo++] = registradores_ptr[i]; 
                long int valorFixo = (long int)params[i].valor.v_ptr;
                for (int j = 0; j < sizeof(long int); j++) {
                    codigo[idxCodigo++] = valorFixo & 0xFF;
                    valorFixo >>= 8;
                }
            }
        } 
        
        else if (params[i].orig_val == IND) {
            
            codigo[idxCodigo++] = 0x48; 
            codigo[idxCodigo++] = 0xbb;
            long int endereco = (long int)params[i].valor.v_ptr;
            for (int j = 0; j < 8; j++) {
                codigo[idxCodigo++] = endereco & 0xFF;
                endereco >>= 8;
            }
            codigo[idxCodigo++] = 0x48; 
            codigo[idxCodigo++] = 0x8b;
            codigo[idxCodigo++] = registradores_ptr[i];
        }
    }

    
    instrucoesFinaisDaPilha(f, codigo, &idxCodigo);

    
    for(int i = 0; i < idxCodigo; i++){
        printf("%hhx\n", codigo[i]);
    }
}
