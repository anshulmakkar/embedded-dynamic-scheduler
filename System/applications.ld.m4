_simple_elf_start = ALIGN(0x4);
. = _simple_elf_start;
INCLUDE "build/app_image.ld";
_simple_elf_end = .;
