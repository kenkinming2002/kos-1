ENTRY(boot_entry)

SECTIONS {
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
  . = 0x01000000;
  . += 0xC0000000;

  . = ALIGN(0x1000);

  .text : AT(ADDR(.text) - 0xC0000000)
  {
    *(.text)
    *(.text.*)
  }

  .rodata : AT(ADDR(.rodata) - 0xC0000000)
  {
    *(.rodata)
    *(.rodata.*)
  }

  . = ALIGN(0x1000);

  .data : AT(ADDR(.data) - 0xC0000000)
  {
    *(.data)
    *(.data.*)
  }

  .bss : AT(ADDR(.bss) - 0xC0000000)
  {
    *(.bss)
    *(.bss.*)
  }

  . -= 0xC0000000;

  boot_memory_begin = .;
}
