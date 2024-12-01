subq $32, %rsp
movq %rdi, -8(%rbp)
movq %rsi, -16(%rbp)
movq %rdx, -24(%rbp)

movq -24(%rbp), %rsi
movq -16(%rbp), %rsi
movq -8(%rbp), %rsi
