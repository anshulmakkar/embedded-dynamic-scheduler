ENTRY(_start)
/* start = .;*/
SECTIONS
{
   .text 0x38000000: 
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
    
    .rtu_data :
    {
      KEEP(*(.rtu_data))
    }
    . = ALIGN(4);
}
