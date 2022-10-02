ENTRY(entry)

SECTIONS {
  /* Mapping */
  BOOT_MAP_BEGIN  = 0x0;
  BOOT_MAP_LENGTH = 0x400000;

  LINEAR_MAP_BEGIN  = 0xC0000000;
  LINEAR_MAP_LENGTH = 0x20000000;

  DYNAMIC_MAP_BEGIN  = 0xD0000000;
  DYNAMIC_MAP_LENGTH = 0x10000000;

  KERNEL_MAP_BEGIN  = 0xE0000000;
  KERNEL_MAP_LENGTH = 0x10000000;

  /* Physical begin and offset */
  BOOT_PHYSICAL_BEGIN   = 0x0;
  LINEAR_PHYSICAL_BEGIN = 0x0;
  KERNEL_PHYSICAL_BEGIN = 0x01000000;

  BOOT_OFFSET   = BOOT_MAP_BEGIN   - BOOT_PHYSICAL_BEGIN;
  LINEAR_OFFSET = LINEAR_MAP_BEGIN - LINEAR_PHYSICAL_BEGIN;
  KERNEL_OFFSET = KERNEL_MAP_BEGIN - KERNEL_PHYSICAL_BEGIN;

  /* Multiboot2 */
  .multiboot2 : { KEEP(*(.multiboot2)) }

  /* Boot */
  . = BOOT_MAP_BEGIN;

  . += 0x00100000; /* Skip over low memory */
  boot_begin = .;
  .boot.text   : AT(ADDR(.boot.text  ) - BOOT_OFFSET) { *(.boot.text   .boot.text.*   ) }
  .boot.rodata : AT(ADDR(.boot.rodata) - BOOT_OFFSET) { *(.boot.rodata .boot.rodata.* ) }
  .boot.data   : AT(ADDR(.boot.data  ) - BOOT_OFFSET) { *(.boot.data   .boot.data.*   ) }
  .boot.bss    : AT(ADDR(.boot.bss   ) - BOOT_OFFSET) { *(.boot.bss    .boot.bss.*    ) }
  boot_end = .;

  /* Kernel */
  . = KERNEL_MAP_BEGIN;

  kernel_begin = .;
  .text   : AT(ADDR(.text  ) - KERNEL_OFFSET) { *(.text   .text.*   ) }
  .rodata : AT(ADDR(.rodata) - KERNEL_OFFSET) { *(.rodata .rodata.* ) }
  .data   : AT(ADDR(.data  ) - KERNEL_OFFSET) { *(.data   .data.*   ) }
  .bss    : AT(ADDR(.bss   ) - KERNEL_OFFSET) { *(.bss    .bss.*    ) }
  kernel_end = .;
}
