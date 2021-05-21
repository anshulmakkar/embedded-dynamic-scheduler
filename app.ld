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
    .rtu_data :
    {
      __RTU_DATA_START = .;
      KEEP(*(.rtu_data))
      __RTU_DATA_END = .;
    }
    . = ALIGN(4);
}
