.section .text
.global _start

_start:
    li x2, 0xF
    csrrwi x0,mepc,0x0
    csrrwi x1, mepc, 0xE
    bne x1, x0, _bad
    csrrw x1, mepc, x0
    addi  x1, x1, 1
    beq x1, x2, _good
    nop
