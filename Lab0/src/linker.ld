SECTIONS
{
  . = 0x80000; /* Start at memory address 0x80000 */
  .text : { *(.text) } /* Place all .text sections here */
}