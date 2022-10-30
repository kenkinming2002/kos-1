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

.section .bss
  stack: .fill STACK_SIZE, 1, 0

  .align 0x1000
  bootstrap_pml4: .fill 0x1000, 1, 0
  bootstrap_pml3: .fill 0x1000, 1, 0
  bootstrap_pml2: .fill 0x1000, 1, 0
  bootstrap_pml1: .fill 0x1000, 1, 0

.section .rodata
  .align 8
  bootstrap_gdt:
    .long 0x0
    .long 0x0

    // Code segment
    .long 0xFFFF
    .byte 0x0
    .byte 0x9A
    .byte 0xAF
    .byte 0x0

  bootstrap_gdt_end:

  .align 8
  bootstrap_gdt_ptr:
    .word bootstrap_gdt_end - bootstrap_gdt - 1
    .quad bootstrap_gdt

.section .text

entry32:
.code32
  xor %ebp, %ebp
  mov $(stack + STACK_SIZE), %esp

  push %eax
  push %ebx

  // At this point we could only use 32 bit registers so setting up identity
  // paging for half of the address space i.e. 128TB would be cumbersome.
  // That's why we only identity map the first 2MB to begin with.
  movl $(bootstrap_pml3 + 0x3), bootstrap_pml4
  movl $(bootstrap_pml2 + 0x3), bootstrap_pml3
  movl $(bootstrap_pml1 + 0x3), bootstrap_pml2

  // Fill PML1
  mov $0x3,            %esi
  mov $bootstrap_pml1, %edi
  mov $0x200,          %ecx
0:
  mov %esi, (%edi)
  add $0x1000, %esi
  add $0x8,    %edi
  loop 0b

  // Point cr3 to PML4
  mov $bootstrap_pml4, %ecx
  mov %ecx, %cr3

  // Enable PAE and PSE
  mov %cr4, %ecx
  or $0x30, %ecx
  mov %ecx, %cr4

  // Enable Long Mode
  mov $0xC0000080, %ecx
  rdmsr
  or $0x100, %eax
  wrmsr

  // Enable Paging
  mov %cr0, %ecx
  or $0x80000000, %ecx
  mov %ecx, %cr0

  // Setup GDT
  lgdt bootstrap_gdt_ptr

  pop %eax
  pop %ebx

  ljmp $0x08, $entry64

entry64:
.code64
  xor %rbp, %rbp
  mov $(stack + STACK_SIZE), %rsp

  mov %ebx, %edi
  mov %eax, %esi
  call bmain

0:
  hlt
  jmp 0b

