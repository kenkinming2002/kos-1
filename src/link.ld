ENTRY(boot_entry)

SECTIONS {

  /* Multiboot 2 */
  .multiboot2 :
  {
    KEEP(*(.multiboot2))
  }

  /* Lower half */
  . = 0x00000500;

  .boot.text :
  {
    *(.boot.text)
    *(.boot.text.*)
  }

  .boot.rodata :
  {
    *(.boot.rodata)
    *(.boot.rodata.*)
  }

  .boot.data :
  {
    *(.boot.data)
    *(.boot.data.*)
  }

  .boot.bss :
  {
    *(.boot.bss)
    *(.boot.bss.*)
  }

  boot_heap_begin = .;

  /* Higher half */
  . = 0x00100000;

  . += 0xC0000000;

  .text ALIGN(0x1000) : AT(ADDR(.text) - 0xC0000000)
  {
    *(.text)
    *(.text.*)
  }

  .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - 0xC0000000)
  {
    *(.rodata)
    *(.rodata.*)
  }

  .data ALIGN(0x1000) : AT(ADDR(.data) - 0xC0000000)
  {
    *(.data)
    *(.data.*)
  }

  .bss ALIGN(0x1000) : AT(ADDR(.bss) - 0xC0000000)
  {
    *(.bss)
    *(.bss.*)
  }

  . -= 0xC0000000;

  boot_memory_begin = .;
}
