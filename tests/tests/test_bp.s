.section .text
.global _start

_start:
    li   x4, 5
    li   x5, 7
    sw   x4, 32(x0)
    #EXE->EXE
    addi x1,x0,2
    addi x2, x1, 3
    nop
    nop
    nop
    bne  x2, x4, _bad
    #EXE->DEC
    addi x8,x0, 2
    beq  x8, x0, _bad
    #MEM->EXE
    lw   x7, 32(x0)
    addi x2, x7, 2
    nop
    nop
    nop
    bne  x2, x5, _bad

    #MEM->DEC
    lw   x2, 32(x10)
    bne  x2, x4, _bad
    j _good
    nop