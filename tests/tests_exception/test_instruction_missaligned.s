.section .text
.global _start

_start :
    addi x1,x0,1
    bne x1,x0,_label
.space 6
_label :
    nop
.space 2
    ori x2,x0,12
    nop
    nop