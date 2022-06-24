.section .text
.global _start

_start:
    addi x4, x0, 0b11110 
    csrrwi x0, 0x300,0x1F # 0x300 = 0....011111
    csrrci x3,0x300, 0x1 # clear the lowest bit
    csrrw x3,0x300,x0 # x3 = 0b11110
    beq x3,x4, _good
    nop
    nop


