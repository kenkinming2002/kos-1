ENTRY(entry)

SECTIONS {
  . = 0x01000000;

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
}
