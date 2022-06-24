.section .text
.global _start

_start:
    addi x3, x0,0x5
    addi x4, x0, 0x5
    csrrs x1, 0x300, x3 # 0x300 <=  (0x5 or 0x0 = 0x5) 
    beq x3,x4, _good
    beq x3,x4, _bad
    nop
    nop

