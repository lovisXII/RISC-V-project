extern void _bad();
extern void _good();

__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x2,x1 ");
__asm__("jal x5, main");


int somme(int* a)
{
    return (*a)++ ;
}
int main() {
    int a = 3;
    int b = 4;
    for (;a > 0; a--) {
        somme(&b);
    }

    if (b == 7) {
        _good();
    }
    _bad();
}