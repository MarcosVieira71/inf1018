

.data
Sf:  .string "%d\n"	/*string de formato para printf */
var: .quad 0       	/* variavel para armazenar o valor */

.text
.globl  main
main:

/********************************************************/
/* mantenha este trecho aqui e nao mexa - prologo !!!   */
  pushq   %rbp
  movq	%rsp, %rbp
  subq	$16, %rsp
  movq	%rbx, -8(%rbp)  /* guarda rbx */
  movq	%r12, -16(%rbp)  /* guarda r12 */
/********************************************************/

  movq	$var, %r12 	/* inicializa %r12 com o endereço de 'var' */
L1:
  cmpb  $10, (%r12)  /* if (*pc == 0) ? */
  je  L2     	/* goto L2 */
 
  addq $1,(%r12)
  mov  (%r12), %eax	/* eax = *r12 (estendendo o byte para 32 bits */
  imul %eax, %eax
/*************************************************************/
/* este trecho imprime o valor de %eax (estraga %eax)  */
  movq	$Sf, %rdi	/* primeiro parametro (ponteiro)*/
  movl	%eax, %esi   /* segundo parametro  (inteiro) */
  call printf
  movl  $0, %eax
  jmp L1
  /*************************************************************/
L2:
/***************************************************************/
/* mantenha este trecho aqui e nao mexa - finalizacao!!!!  	*/
  movq  $0, %rax  /* rax = 0  (valor de retorno) */
  movq	-16(%rbp), %r12 /* recupera r12 */
  movq	-8(%rbp), %rbx  /* recupera rbx */
  leave
  ret 	 
/***************************************************************/


