#include "defs.h"

#define MULTIBOOT2_MAGIC         0xE85250D6
#define MULTIBOOT2_ARCHITECTURE  0x0
#define MULTIBOOT2_HEADER_LENGTH multiboot2_header_end - multiboot2_header_begin
#define MULTIBOOT2_CHECKSUM      -(MULTIBOOT2_MAGIC+MULTIBOOT2_HEADER_LENGTH+MULTIBOOT2_ARCHITECTURE)

.global entry
.extern kmain

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

/* Bootstrap data */
.section .boot.bss, "aw", @nobits
  .align PAGE_SIZE
  bootstrap_stack: .fill PAGE_SIZE, 1, 0
  bootstrap_pd:    .fill PAGE_SIZE, 1, 0

/* Bootstrap code */
.section .boot.text, "ax", @progbits
  entry:
    xor %ebp, %ebp
    mov $(bootstrap_stack + PAGE_SIZE), %esp

    push %eax
    push %ebx

    mov $(BOOT_MAP_BEGIN      \/ LARGE_PAGE_SIZE), %eax
    mov $(BOOT_MAP_LENGTH     \/ LARGE_PAGE_SIZE), %ebx
    mov $(BOOT_PHYSICAL_BEGIN \/ LARGE_PAGE_SIZE), %ecx
    call do_linear_map

    mov $(LINEAR_MAP_BEGIN      \/ LARGE_PAGE_SIZE), %eax
    mov $(LINEAR_MAP_LENGTH     \/ LARGE_PAGE_SIZE), %ebx
    mov $(LINEAR_PHYSICAL_BEGIN \/ LARGE_PAGE_SIZE), %ecx
    call do_linear_map

    mov $(KERNEL_MAP_BEGIN      \/ LARGE_PAGE_SIZE), %eax
    mov $(KERNEL_MAP_LENGTH     \/ LARGE_PAGE_SIZE), %ebx
    mov $(KERNEL_PHYSICAL_BEGIN \/ LARGE_PAGE_SIZE), %ecx
    call do_linear_map

    mov %cr4, %eax
    or $0x00000010, %eax
    mov %eax, %cr4

    mov $bootstrap_pd, %eax
    mov %eax, %cr3

    mov %cr0, %eax
    or  $0x80000001, %eax
    mov %eax, %cr0

    push %ebx
    push %eax

    jmp higher_half

  /* eax: virtual index
   * ebx: count
   * ecx: physical index */
  do_linear_map:
    mov %ecx,              %edx
    shl $LARGE_PAGE_SHIFT, %edx
    or  $0x83,             %edx
    mov %edx, bootstrap_pd(,%eax,4)

    inc %eax
    inc %ecx
    dec %ebx
    jnz do_linear_map
    ret

/* Higher half code */
.section .bss
  .align PAGE_SIZE
  initial_stack: .fill PAGE_SIZE, 1, 0

.section .text
  higher_half:
    xor %ebp, %ebp
    mov $(initial_stack + PAGE_SIZE), %esp

    push %eax
    push %ebx
    call kmain
  0:
    hlt
    jmp 0b
