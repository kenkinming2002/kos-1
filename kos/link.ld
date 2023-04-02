ENTRY(kmain)

SECTIONS {
  . = ALIGN(0x1000);
  .text   : { *(.text   .text.*   ) }
  .rodata : { *(.rodata .rodata.* ) }
  .module :
  {
    module_begin = .;
    KEEP(*(.module))
    module_end   = .;
  }

  .data   : { *(.data   .data.*   ) }
  .bss    : { *(.bss    .bss.*    ) }
}
