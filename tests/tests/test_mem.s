.section .text
.global _start

_start:
    addi x1,x0,12
    ori x1,x0,1953
    slli x1,x1,6 # x1 = 124992
    ori x1,x1,20 # x1 = 125012
    ori x2,x0,84 #less significant byte of 125012
    sw x1,32(x0)
    lw x3,32(x0) # x3 = 125012
    sb x2,36(x0)  
    lb x4,36(x0) # x4 = 84
    sub x5,x3,x4 #125012 - 84 = 124928
    ori x6,x0,61
    sll x6,x6,11 # sll = 124928
    bne x5,x6, _bad
    nop
    jal x6, _good
    nop
    nop


