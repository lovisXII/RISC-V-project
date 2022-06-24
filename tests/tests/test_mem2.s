.section .text
.global _start

_start:

    addi x2,x0,2020 # 0x7E4
    sw x2,4(x0)
    lbu x3,4(x0) 
    addi x4, x0, 0xE4
    beq x3,x4,_good
    nop 



