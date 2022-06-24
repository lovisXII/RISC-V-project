.section .text
.global _start

_start:
    addi x6,x0,8 #x6 = 8
    addi x7,x0,12 # x7 = 12
    addi x9,x0,16 # x9 = 16

_loop :
    addi x8,x8,2 # x8 = x7 + 4
    bne x8,x9,_loop
    beq x8,x9,_good
    nop