.global entry
.extern kmain

/* Multiboot 2 header */
.section .multiboot2

.equ MULTIBOOT2_MAGIC,         0xE85250D6
.equ MULTIBOOT2_ARCHITECTURE,  0x0
.equ MULTIBOOT2_HEADER_LENGTH, multiboot2_header_end - multiboot2_header_begin
.equ MULTIBOOT2_CHECKSUM,      -(MULTIBOOT2_MAGIC+MULTIBOOT2_HEADER_LENGTH+MULTIBOOT2_ARCHITECTURE)

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

/* Constant */
.equ PAGE_SIZE,       0x1000

/* Bootstrap data */
.section .bss

.align PAGE_SIZE
bootstrap_stack: .fill PAGE_SIZE, 1, 0

/* Bootstrap code */
.section .text
entry:
  /* Load the stack */
  xor %ebp, %ebp
  mov $(bootstrap_stack + PAGE_SIZE), %esp

  /* Call kmain */
  push %ebx
  push %eax
  call kmain

.loop:
  hlt
  jmp .loop
