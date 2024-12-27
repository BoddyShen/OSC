#include "mm.h"

.section ".text.boot"

.globl _start
_start:
    mrs    x0, mpidr_el1       // Load processor id from Multi-Processor ID Register
    and    x0, x0,#0xFF        // Check processor id
    cbz    x0, master          // Hang for all non-primary CPU
    b    proc_hang

proc_hang:
    wfe // Wait for event
    b proc_hang

master:
    adr    x0, bss_begin
    adr    x1, bss_end
    sub    x1, x1, x0
    bl     memzero

    mov    sp, #LOW_MEMORY
    bl    kernel_main

.align 4
memzero:
	str xzr, [x0], #8         // xzr always be 0
	subs x1, x1, #8
	b.gt memzero              // if x1 > 0, do memzero
	ret