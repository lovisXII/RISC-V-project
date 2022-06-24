.section .text
.global _start

_start:
    li x2, 0
    li x3, 4
    jal x1, _func
    jal x1, _func
    jal x1, _func
    jal x1, _func
    beq x2, x3, _good
    j _bad

_func :
    addi x2,x2,1 
    ret

    nop
    nop
    nop
