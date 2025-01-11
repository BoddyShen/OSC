SECTIONS
{
    . = 0x3000000;
    .text.boot : { *(.text.boot) } 
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : {
        . = ALIGN(16);
        bss_begin = .;
        *(.bss*)
        bss_end = .;
    }
}