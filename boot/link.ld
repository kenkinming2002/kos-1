ENTRY(entry32)

SECTIONS {
  /* Multiboot2 */
  .multiboot2 : { KEEP(*(.multiboot2)) }

  . = 0x00100000; /* Skip over low memory */

  .text   : { *(.text   .text.*   ) }
  .rodata : { *(.rodata .rodata.* ) }
  .data   : { *(.data   .data.*   ) }
  .bss    : { *(.bss    .bss.*    ) }
}
