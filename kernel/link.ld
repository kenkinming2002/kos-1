ENTRY(kmain)

SECTIONS {
  kernel_begin = .;
  .text   : { *(.text   .text.*   ) }
  .rodata : { *(.rodata .rodata.* ) }
  .data   : { *(.data   .data.*   ) }
  .bss    : { *(.bss    .bss.*    ) }
  kernel_end = .;
}
