.section .text
.global _start

_start:
    addi x1,x0,1254
    bge x0,x0,_good
    nop



# 10134:	00e7c863          	blt	a5,a4,10144 <main+0x88>
# 0 000000 01110 01111 100 1000 0 1100011
# codé comme un blt
# blt r15, r14, offset
# 100a4:	fc07dee3          	bgez	a5,10080 <modulo+0x18>
# 1 111110 00000 01111 101 1110 1 1100011
# bge r15,r0, offset
# codé comme un bge