.section .text
.global _start

_start:
    ori x1,x0,10 # x1 = 10 // 1010 
    ori x2,x0,12 # x2 = 12 // 1100
    ori x3,x0,1 # x3 = 1
    slt x4,x1,x3 # x4 = 0 
    sltu x4,x1,x1 # x4 = 1
    and x5,x3,x3 # x5 = 1 
    or x6,x1,x2 # x6 = 1110
    xor x7,x4,x5 # x7 = 1
    sll x8,x2,x3 # 12 <<1 = 24
    srl x9,x2,x3 # 6
    srl x10,x1,x3 # 5
    sra x11,x1,x3 # 5

    add x12,x11,x10 # 10
    add x13,x9,x8 # 30
    add x14,x7,x6 # 1+13 = 14
    add x15,x12,x13 # 40
    add x16, x15,x14 # 54
    sub x17,x16,x16 
    bne x17, x0, _bad
    add x1, x1, x2
    beq x17, x0, _good
    nop
    nop