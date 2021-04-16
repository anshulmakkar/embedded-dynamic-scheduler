ENTRY(_start)
/* start = .;*/
SECTIONS
{
    . = SEGMENT_START("text-segment", 0) + SIZEOF_HEADERS;
   .text : 
    {
        *(.text)
    }
    .fini :
    {
        KEEP (*(.fini))
    } =0
    .dynamic : { *(.dynamic) }
    got : { *(.got.plt) *(.igot.plt) *(.got) *(.igot) }
    .data :
    {
        *(.data)
        *(.rodata)
    }
    
    .bss :
    {
        *(.bss)
    }
    . = ALIGN(4);
}
