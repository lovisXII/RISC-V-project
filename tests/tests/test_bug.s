.section .text
.global _start

#This programm bug, indeed the ori x1,x1,20 instruction should give 125012 as result
#but it gaves 1973, because it doesn't correctly wait for x1 to be wbk

_start:
    ori x1, x0, 3
    ori x1, x0, 4
    or x2, x1, x0
    bne x2,x1, _bad
    beq x1,x2, _good
    nop
    nop


