.section .text
.global _start

_start:
    auipc x1, 12
    j _good
    nop
    nop
    nop
    nop
    nop