#include<stdio.h>
extern void _bad();
extern void _good();
__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x2,x1 ");
__asm__("jal x5, main");
int main(){
    printf("test\n") ;
    _good() ;
}