SECTIONS
{
    . = 0x60000;
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
    . = 0x3000000;
    _kernel_start_addr = 0x3000000;
}