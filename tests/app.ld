
/**********************************************************
        File : 
        Author : 
        Date : 
**********************************************************/

/* Grouping sections into segments for the link editor. */


SECTIONS
{
    . = 0x10054 ;
    seg_text :
    {
        *(.text)
    }
    . = 0x80000000 ;
    seg_reset :
    {
        *(.reset)
    }
    . = 0x81000000 ;
    seg_kernel :
    {
        *(.kernel) 
    }
}

INPUT(kernel)
