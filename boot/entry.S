.global entry32
.extern bmain

#define STACK_SIZE 0x1000

.section .bss
  stack: .fill STACK_SIZE, 1, 0

.section .bss
  .align 0x1000
  bootstrap_pml4: .fill 0x1000,   1, 0
  bootstrap_pml3: .fill 0x100000, 1, 0 // 1GiB

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

  mov $bootstrap_pml4, %edi
  mov $(bootstrap_pml3 + 0x3), %eax
  xor %edx, %edx
  mov $0x1000, %ebx
  mov $0x100,  %ecx
  call pml_init

  mov $bootstrap_pml3, %edi
  mov $0x83, %eax
  xor %edx, %edx
  mov $0x40000000, %ebx
  mov $0x20000,  %ecx
  call pml_init

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

.section .text
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

// edi:     address of pml*
// eax:edx: address to map with required mask
// ebx:     increment
// ecx:     count
.section .text
pml_init:
.code32
0:
  mov %eax, 0(%edi)
  mov %edx, 4(%edi)
  add $0x8, %edi
  add %ebx, %eax
  jnc 1f
  inc %edx
1:
  loop 0b
  ret

