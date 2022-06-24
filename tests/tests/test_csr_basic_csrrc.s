.section .text
.global _start

_start:
    addi x3, x0,0x5
    addi x4, x0, 0x4
    addi x5,x0,0x1 #mask
    csrrwi x1, 0x300, 0x5 #0x300 <= 0x5
    nop
    csrrc x3,0x300, x5 #0x300 <= 0x5 and not(0x1)
    csrrwi x3,0x300,0x0
    beq x3,x4, _good
    beq x3,x4, _bad
    nop
    nop


