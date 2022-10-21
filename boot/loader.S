#define STACK_SIZE 0x1000

#define MULTIBOOT2_MAGIC         0xE85250D6
#define MULTIBOOT2_ARCHITECTURE  0x0
#define MULTIBOOT2_HEADER_LENGTH multiboot2_header_end - multiboot2_header_begin
#define MULTIBOOT2_CHECKSUM      -(MULTIBOOT2_MAGIC+MULTIBOOT2_HEADER_LENGTH+MULTIBOOT2_ARCHITECTURE)

.global entry
.extern bmain

/* Multiboot 2 header */
.section .multiboot2
  .align 8
  multiboot2_header_begin:
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

.section .bss, "aw", @nobits
  stack: .fill STACK_SIZE, 1, 0

.section .text
  entry:
    xor %ebp, %ebp
    mov $(stack + STACK_SIZE), %esp

    push %ebx
    push %eax
    call bmain
  0:
    hlt
    jmp 0b
