ENTRY(boot_entry)

SECTIONS {
  . = 0x00100000;

  /* Multiboot 2 */
  .multiboot2 :
  {
    KEEP(*(.multiboot2))
  }

  /* Higher half */
  . += 0xC0000000;

  .text ALIGN(0x1000) : AT(ADDR(.text) - 0xC0000000)
  {
    *(.text)
  }

  .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - 0xC0000000)
  {
    *(.rodata)
  }

  .data ALIGN(0x1000) : AT(ADDR(.data) - 0xC0000000)
  {
    *(.data)
  }

  .bss ALIGN(0x1000) : AT(ADDR(.bss) - 0xC0000000)
  {
    *(.bss)
  }

  /* Lower half */
  . -= 0xC0000000;

  .boot.text ALIGN(0x1000) :
  {
    *(.boot.text)
  }

  .boot.rodata ALIGN(0x1000) :
  {
    *(.boot.rodata)
  }

  .boot.data ALIGN(0x1000) :
  {
    *(.boot.data)
  }

  .boot.bss ALIGN(0x1000) :
  {
    *(.boot.bss)
  }
}
