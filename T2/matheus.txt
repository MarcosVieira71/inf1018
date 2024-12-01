#include <stdio.h>
#include <stdlib.h>

#include "cria_func.h"

// Arrumar os códigos de exit
// Supondo que só exista um parâmetro variável
void cria_func (void* f, DescParam params[], int n, unsigned char codigo[]){
   
    // Se forem recebidos mais parâmetros que o permitido
    if (n>3){
        fprintf(stderr, "Erro: foram passados mais parâmetro que o percebido.");
        exit(1);
    }

    int param_lidos = 0;

    // pushq %rbp
    *codigo++ = 0x55;

    // movq %rsp, %rbp
    *codigo++ = 0x48;
    *codigo++ = 0x89;
    *codigo++ = 0xe5;

    // subq $32, %rsp
    *codigo++ = 0x48;
    *codigo++ = 0x83;
    *codigo++ = 0xec;
    *codigo++ = 0x20;

    // movq %rdi, -8(%rbp)
    *codigo++ = 0x48;
    *codigo++ = 0x89;
    *codigo++ = 0x7d;
    *codigo++ = 0xf8;

    // movq %rsi, -16(%rbp)
    *codigo++ = 0x48;
    *codigo++ = 0x89;
    *codigo++ = 0x75;
    *codigo++ = 0xf0;

    // movq %rdx, -24(%rbp)
    *codigo++ = 0x48;
    *codigo++ = 0x89;
    *codigo++ = 0x55;
    *codigo++ = 0xe8;

    // Percorre os parâmetros recebidos
    for(int i=0; i<n; i++){
        
        // Se o parâmetro for inteiro
        if (params[i].tipo_val == INT_PAR){
           
            // Se o parâmetro for passado na chamada da função
            if (params[i].orig_val == PARAM){

                // movl %valorRdi, %valorRegistradorCorrespondente
                // Move o valor do parâmetro recebido para o registrador

                // movq -?(%rbp), %rax
                *codigo++ = 0x48;
                *codigo++ = 0x8b;
                *codigo++ = 0x45;

                if (param_lidos == 0) *codigo++ = 0xf8; // Se for primeiro parâmetro na nova função (-8)
                else if (param_lidos == 1) *codigo++ = 0xf0; // Se for segundo parâmetro na nova função (-16)
                else if (param_lidos == 2) *codigo++ = 0xe8; // Se for terceiro parâmetro na nova função (-24)

                // movl
                *codigo++ = 0x89;

                if (i == 0) *codigo++ = 0xc7; // Se for o primeiro parâmetro na função origem (%rax), %edi
                else if (i == 1) *codigo++ = 0xc6; // Se for o segundo parâmetro na função origem (%rax), %esi
                else if (i == 2) *codigo++ = 0xc2; // Se for o terceiro parâmetro na função origem (%rax), %edx

                // Atualiza o número de parâmetros lidos da nova função
                param_lidos++;
            }
            
            // Se o parâmetro for fixo
            else if (params[i].orig_val == FIX){

                // movl $valor, %valorRegistradorCorrespondente
                // Move o valor recebido em params para o registrador

                // Se for o primeiro parâmetro na função origem
                if (i == 0) *codigo++ = 0xbf; // %edi

                // Se for o segundo parâmetro na função origem
                else if (i == 1) *codigo++ = 0xbe; // %esi

                // Se for o terceiro parâmetro na função origem
                else if (i == 2) *codigo++ = 0xba; // %edx

                // $valor
                *(int*)codigo = params[i].valor.v_int; 
                codigo += sizeof(int);

            }
            
            // Se o parâmetro estiver no endereço de memória especificado no parâmetro
            else if (params[i].orig_val == IND){

                // movl *params.valor.v_ptr, %valorRegistradorCorrespondente
                // Move o valor recebido por um ponteiro em params para o registrador

                // Move o valor imediatamente depois para %rax
                *codigo++ = 0x48;
                *codigo++ = 0xb8;

                // Insere o conteúdo do ponteiro em %rax
                *(unsigned long*)codigo = (unsigned long)params[i].valor.v_ptr;
                codigo += sizeof(unsigned long*);

                // movl
                *codigo++ = 0x8b;

                /// Se for o primeiro parâmetro na função origem
                if (i == 0) *codigo++ = 0x38; // %edi

                // Se for o segundo parâmetro na função origem
                else if (i == 1) *codigo++ = 0x30; // %esi

                // Se for o terceiro parâmetro na função origem
                else if (i == 2) *codigo++ = 0x10; // %edx

            }
            
            // Caso de erro
            else {
                fprintf(stderr, "Erro: origem de parâmetro não conhecida.");
                exit(1);
            }
        
        }
        
        // Se o parâmetro for ponteiro
        else if(params[i].tipo_val == PTR_PAR){

            // Se o parâmetro for passado na chamada da função
            if (params[i].orig_val == PARAM){
            
                // %ptrRegistrador = %ptrRegistrador
                // Move um ponteiro recebido para o registrador

                // movq -?(%rbp), %rax
                *codigo++ = 0x48;
                *codigo++ = 0x8b;
                *codigo++ = 0x45;

                if (param_lidos == 0) *codigo++ = 0xf8; // Se for primeiro parâmetro na nova função (-8)
                else if (param_lidos == 1) *codigo++ = 0xf0; // Se for segundo parâmetro na nova função (-16)
                else if (param_lidos == 2) *codigo++ = 0xe8; // Se for terceiro parâmetro na nova função (-24)
                
                // movq
                *codigo++ = 0x48;
                *codigo++ = 0x89;

                if (i == 0) *codigo++ = 0xc7; // Se for o primeiro parâmetro na função origem %rax, %rdi
                else if (i == 1) *codigo++ = 0xc6; // Se for o segundo parâmetro na função origem %rax, %rsi
                else if (i == 2) *codigo++ = 0xc2; // Se for o terceiro parâmetro na função origem %rax, %rdx
                
                // Atualiza o número de parâmetros lidos da nova função
                param_lidos++;
            }
            
            // Se o parâmetro for fixo
            else if (params[i].orig_val == FIX){
                
                // params[i].valor.vlr_ptr = %ponteiroRegistrador
                // Move o valor do ponteiro em params para o registrador

                // Move o valor imediatamente depois para %rax
                *codigo++ = 0x48;
                *codigo++ = 0xb8;

                // Insere o conteúdo do ponteiro em %rax
                *(unsigned long*)codigo = (unsigned long)params[i].valor.v_ptr;
                codigo += sizeof(unsigned long*);

                // movq
                *codigo++ = 0x48;
                *codigo++ = 0x89;

                // Se for o primeiro parâmetro na função origem
                if (i == 0) *codigo++ = 0xc7; // %rax, %rdi

                // Se for o segundo parâmetro na função origem
                if (i == 1) *codigo++ = 0xc6; // %rax, %rsi

                // Se for o terceiro parâmetro na função origem
                if (i == 2) *codigo++ = 0xc2; // %rax, %rdx
            }
            
            // Se o parâmetro estiver no endereço de memória especificado no parâmetro
            else if (params[i].orig_val == IND){
                
                // Move o ponteiro recebido em params para o registrador
                // movq *params[i].orig.valor.vlr_ptr = %ponteiroRegistrador

                // Move o valor imediatamente depois para %rax
                *codigo++ = 0x48;
                *codigo++ = 0xb8;

                // Insere o ponteiro em %rax
                *(unsigned long*)codigo = (unsigned long)params[i].valor.v_ptr;
                codigo += sizeof(unsigned long*);
                    
                // movq
                *codigo++ = 0x48;
                *codigo++ = 0x8b;

                // Se for o primeiro parâmetro na função origem
                if (i == 0) *codigo++ = 0x38; // %rax, %rdi

                // Se for o segundo parâmetro na função origem
                if (i == 1) *codigo++ = 0x30; // %rax, %rsi

                // Se for o terceiro parâmetro na função origem
                if (i == 2) *codigo++ = 0x10; // %rax, %rdx
            }
            
            // Caso de erro
            else {
                fprintf(stderr, "Erro: origem de parâmetro não conhecida.");
                exit(1);
            }
        
        }
        
        // Caso de erro
        else{
            fprintf(stderr, "Erro: tipo de parâmetro não conhecido.");
            exit(1);
        }
    
    }

    // movq
    *codigo++ = 0x48;
    *codigo++ = 0xb8;
    // Passa o endereço da função para o registrador
    *(unsigned long*)codigo = (unsigned long)f;
    codigo += sizeof(unsigned long*);

    // Aciona a função
    *codigo++ = 0xff;
    *codigo++ = 0xd0;

    // movq -8(%rbp), %rdi
    *codigo++ = 0x48;
    *codigo++ = 0x8b;
    *codigo++ = 0x7d;
    *codigo++ = 0xf8;

    // movq -16(%rbp), %rsi
    *codigo++ = 0x48;
    *codigo++ = 0x8b;
    *codigo++ = 0x75;
    *codigo++ = 0xf0;

    // movq -24(%rbp), %rdx
    *codigo++ = 0x48;
    *codigo++ = 0x8b;
    *codigo++ = 0x55;
    *codigo++ = 0xe8;

    // Finalização
    *codigo++ = 0xc9;
    *codigo++ = 0xc3;
}