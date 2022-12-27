ENTRY(entry32)

SECTIONS {
  /* Multiboot2 */
  .multiboot2 : { KEEP(*(.multiboot2)) }

  . = 0x00100000; /* Skip over low memory */

  boot_begin = .;
  .text   : { *(.text   .text.*   ) }
  .rodata : { *(.rodata .rodata.* ) }
  .data   : { *(.data   .data.*   ) }
  .bss    : { *(.bss    .bss.*    ) }

  system_structures_begin = .;
  .system_structures (NOLOAD) : { *(.system_structures) }
  system_structures_end = .;

  kernel_begin = .;
  .kernel (NOLOAD) : { *(.kernel) }
  kernel_end = .;

  kboot_begin = .;
  .kboot   (NOLOAD) : { *(.kboot) }
  kboot_end = .;

  boot_end = .;
}
