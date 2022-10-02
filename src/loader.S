.global entry
.extern kmain

#define PAGE_SIZE       0x1000
#define LARGE_PAGE_SIZE 0x400000

#define PAGE_SHIFT       12
#define LARGE_PAGE_SHIFT 22

#define BOOT_MAP_BEGIN  0x0
#define BOOT_MAP_LENGTH 0x400000

#define LINEAR_MAP_BEGIN  0xC0000000
#define LINEAR_MAP_LENGTH 0x20000000

#define DYNAMIC_MAP_BEGIN  0xD0000000
#define DYNAMIC_MAP_LENGTH 0x10000000

#define KERNEL_MAP_BEGIN  0xE0000000
#define KERNEL_MAP_LENGTH 0x10000000

#define BOOT_PHYSICAL_BEGIN   0x0
#define LINEAR_PHYSICAL_BEGIN 0x0
#define KERNEL_PHYSICAL_BEGIN 0x01000000

#define BOOT_OFFSET   BOOT_MAP_BEGIN   - BOOT_PHYSICAL_BEGIN
#define LINEAR_OFFSET LINEAR_MAP_BEGIN - LINEAR_PHYSICAL_BEGIN;
#define KERNEL_OFFSET KERNEL_MAP_BEGIN - KERNEL_PHYSICAL_BEGIN

#define MULTIBOOT2_MAGIC         0xE85250D6
#define MULTIBOOT2_ARCHITECTURE  0x0
#define MULTIBOOT2_HEADER_LENGTH multiboot2_header_end - multiboot2_header_begin
#define MULTIBOOT2_CHECKSUM      -(MULTIBOOT2_MAGIC+MULTIBOOT2_HEADER_LENGTH+MULTIBOOT2_ARCHITECTURE)

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
