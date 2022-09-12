.global entry
.extern bmain

.equ MULTIBOOT2_MAGIC,         0xE85250D6
.equ MULTIBOOT2_ARCHITECTURE,  0x0
.equ MULTIBOOT2_HEADER_LENGTH, multiboot2_header_end - multiboot2_header_begin
.equ MULTIBOOT2_CHECKSUM,      -(MULTIBOOT2_MAGIC+MULTIBOOT2_HEADER_LENGTH+MULTIBOOT2_ARCHITECTURE)

.section .multiboot2

multiboot2_header_begin:

.align 8

.long MULTIBOOT2_MAGIC
.long MULTIBOOT2_ARCHITECTURE
.long MULTIBOOT2_HEADER_LENGTH
.long MULTIBOOT2_CHECKSUM

.short 1
.short 0
.long  12
.long  6
.long  0

.short 0
.short 0
.long  8

multiboot2_header_end:

.equ KERNEL_STACK_SIZE, 4096

.section .bss

kernel_stack:
.fill KERNEL_STACK_SIZE, 1, 0

.section .text
entry:
  xor %ebp, %ebp
  mov $(kernel_stack + KERNEL_STACK_SIZE), %esp

  push %ebx
  push %eax
  call bmain

.loop:
  hlt
  jmp .loop
