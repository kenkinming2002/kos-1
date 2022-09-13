.global entry
.extern bmain

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
.equ PAGE_SIZE_HALF,  0x800
.equ PAGE_SIZE,       0x1000
.equ PAGE_OFFSET,     0xC0000000

/* Bootstrap data */
.section .bss

.align PAGE_SIZE
bootstrap_pd:    .fill PAGE_SIZE, 1, 0
bootstrap_stack: .fill PAGE_SIZE, 1, 0

/* Bootstrap code */
.section .text
entry:
  /* We cannot use eax and ebx as they contain information from multiboot2 */

  /* Initialize page directory that maps 0GB-2GB to both 0GB-2GB and 2GB-4GB.
   * Total number of entries is 1024. 512 for each mappings range respecitvely. */
  xor %ecx, %ecx

1:
  mov %ecx, %edx
  shl $22, %edx
  or $0b000010000011, %edx

  mov %edx, (bootstrap_pd + 0             )(,%ecx, 4)
  mov %edx, (bootstrap_pd + PAGE_SIZE_HALF)(,%ecx, 4)

  inc %ecx
  cmp $512, %ecx
  jne 1b

  /* Enable PSE(Page Size Extension) */
  mov %cr4, %ecx
  or $0x00000010, %ecx
  mov %ecx, %cr4

  /* Load the page directory */
  mov $bootstrap_pd, %ecx
  mov %ecx, %cr3

  /* Enable paging */
  mov %cr0, %ecx
  or $0x80000001, %ecx
  mov %ecx, %cr0

  /* Load the stack */
  xor %ebp, %ebp
  mov $(bootstrap_stack + PAGE_SIZE), %esp

  push %ebx
  push %eax
  call bmain

.loop:
  hlt
  jmp .loop
