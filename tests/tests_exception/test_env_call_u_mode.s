.section .text
.global _start


_start :
    la x29,_trampoline    #loading exception handler adress
    srli x29,x29,2       # removing least 2 significant bits to allow 
    sll x29,x29,2
    addi x29,x29,1       # direct mode for mtvec 
    csrrw x0, 0x305,x29  # writting exception handler adress in mtvec
    ecall
    nop

_trampoline: 
   j _bad
   j _bad
   j _bad
   j _bad
   j _bad
   j _bad
   j _bad
   j _bad
   j _good
   j _bad
   j _bad
   j _bad
   j _bad
   j _bad
