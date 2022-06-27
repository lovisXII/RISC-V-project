/*      
extern void _bad();
extern void _good();
*/
__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x2,x1 ");
__asm__("jal x5, main");

int modulo(int a, int b){
    while(a >= b) a -= b;
    return a ;
}


void _bad() {
    __asm__("ori x20, x0, 0x0CC");
    __asm__("nop");
}

void _good() {
    __asm__("nop");
    __asm__("nop");
}

int main()
{
    int nbr1, nbr2 ;
    int pgcd, i;
	
	nbr1 = 3 ;
    nbr2 = 7 ;
    
    for(i=1; i <= nbr1 && i <= nbr2; ++i)
    {
        if(modulo(nbr1,i) == 0 && modulo(nbr2,i) == 0)
            pgcd = i;
    }
	if(pgcd == 1)
    {
        _good() ;
    }
    else{
        _bad() ;
    }
}