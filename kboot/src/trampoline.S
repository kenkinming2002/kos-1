.extern ap_entry

.section .trampoline, "ax", @progbits
.code16
  // Clear interrupt and direction flags
  cli
  cld

  // Initialize segment registers in real mode
  ljmp $0x0, $0f
0:

  xor %ax, %ax
  mov %ax, %ss
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  // Load GDT and enter protected mode
  lgdt trampoline_gdt_ptr
  mov %cr0, %eax
  or  $0x1, %eax
  mov %eax, %cr0

  // Initialize segment registers in protected mode
  ljmp $0x8, $0f

.code32
0:
  mov $0x10, %ax
  mov %ax, %ss
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  // Call entry
  xor %ebx, %ebx
  jmp ap_entry

  .align 8
  trampoline_gdt:
    .quad 0x0                // Null segment
    .quad 0x00CF9A000000FFFF // 32 bit Code segment
    .quad 0x008F92000000FFFF // 32 bit Data segment
  trampoline_gdt_end:

  .align 8
  trampoline_gdt_ptr:
    .word trampoline_gdt_end - trampoline_gdt - 1
    .quad trampoline_gdt

