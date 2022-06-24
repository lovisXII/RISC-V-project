.section .text
.global _start

_start :
    li x3,0x90000000
    lw x4,0(x3)
