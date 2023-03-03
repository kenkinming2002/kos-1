ENTRY(kmain)

SECTIONS {
  . = ALIGN(0x1000);
  .text   : { *(.text   .text.*   ) }
  .rodata : { *(.rodata .rodata.* ) }

  . = ALIGN(0x1000);
  module_begin = .; .module : SUBALIGN(0) { KEEP(*(.module)) } module_end = .;

  .data   : { *(.data   .data.*   ) }
  .bss    : { *(.bss    .bss.*    ) }
}
