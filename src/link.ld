ENTRY(entry)

SECTIONS {
  . = 0x01000000;

  kernel_begin = .;

  .text :
  {
    *(.text)
    *(.text.*)
  }

  .rodata :
  {
    *(.rodata)
    *(.rodata.*)
  }

  .data :
  {
    *(.data)
    *(.data.*)
  }

  .bss :
  {
    *(.bss)
    *(.bss.*)
  }

  kernel_end = .;
}
