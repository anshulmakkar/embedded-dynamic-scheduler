_simple_elf_start = ALIGN(0x4);
_simple_elf_start = .;
INCLUDE "App/app_image.ld";
_simple_elf_end = .;
