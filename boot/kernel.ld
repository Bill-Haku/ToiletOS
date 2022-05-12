ENTRY(boot_start)
SECTIONS
{
    /*内核是加载到1 M空间之上的*/
    . = 0x100000;
    .text :
    {
        *(.text)
        . = ALIGN(4096);
    }

    .data :
    {
        *(.data)
        . = ALIGN(4096);
    }
    .rodata :
    {
        *(.rodata)
        . = ALIGN(4096);    
    }
    .bss :
    {
        *(.bss)
        . = ALIGN(4096);
    }

    .stab :
    {
        *(.stab)
        . = ALIGN(4096);
    }
    
    .stabstr :
    {
        *(.stabstr)
        . = ALIGN(4096);
    }

    /DISCARD/ : { *(.comment) *(.eh_frame) }
}