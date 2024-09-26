.data
nums: .int 65, -105, 111, 34
Sf: .string "%d\n" # string de formato para printf
.text
.global main
main:
/********************************************************/
/* mantenha este trecho aqui e nao mexa - prologo !!! */
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movq %rbx, -8(%rbp)
    movq %r12, -16(%rbp)
    /********************************************************/
    movl $0, %ebx /* ebx = 0; */
    movl $0, %eax
    movq $nums, %r12 /* r12 = &nums */
L1:
    cmpl $4, %ebx /* if (ebx == 4) ? */
    je L2
    movq %r12, %r13
    movl $4, %edx 
    imull %ebx, %edx 
    movslq %edx, %rdx 
    addq %rdx, %r13 
    addl (%r13), %eax 
    addl $1, %ebx
    jmp L1
L2:
    /*************************************************************/
    /* este trecho imprime o valor de %eax (estraga %eax) */
    movq $Sf, %rdi /* primeiro parametro (ponteiro)*/
    movl %eax, %esi /* segundo parametro (inteiro) */
    call printf /* chama a funcao da biblioteca */
    /*************************************************************/
    jmp L3
L3:
    /***************************************************************/
    /* mantenha este trecho aqui e nao mexa - finalizacao!!!! */
    movq $0, %rax /* rax = 0 (valor de retorno) */
    movq -8(%rbp), %rbx
    movq -16(%rbp), %r12
    leave
    ret
