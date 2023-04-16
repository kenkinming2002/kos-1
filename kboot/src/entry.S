.global entry

.extern boot_info
.extern bmain

#define STACK_SIZE  0x1000

/********************
 * Global Variables *
 ********************/
.section .system_structures, "aw", @nobits
  .align 0x1000
  stack:
    .fill STACK_SIZE, 1, 0
  stack_end:

.section .system_structures, "aw", @nobits
  .align 0x1000
  pml4: .fill 512,       8, 0
  pml3: .fill 512 * 512, 8, 0

.section .rodata
  .align 8
  gdt:
    .quad 0x0                // Null segment
    .quad 0x00209A0000000000 // 64 bit Code segment
    .quad 0x0000920000000000 // 64 bit Data segment
  gdt_end:

  .align 8
  gdt_ptr:
    .word gdt_end - gdt - 1
    .quad gdt

.section .data
stack_ptr:
  .long stack_end

/*******************
 * Entry point *
 *******************/
.section .text
entry:
.code32
  cmp $0x36d76289, %eax
  je 1f

0:
  hlt
  jmp 0b

1:
.code32
  /*********************
   * Initialize Paging *
   *********************/
  mov $(pml3 + 0x3), %eax
  mov $pml4, %edi
  mov $512, %ecx
0:
  mov %eax, 0(%edi)
  movl $0,   4(%edi)
  add $0x8,    %edi
  add $0x1000, %eax
  loop 0b

  mov $0x83, %eax
  xor %edx, %edx
  mov $pml3, %edi
  mov $(512 * 512), %ecx
0:
  mov %eax, 0(%edi)
  mov %edx, 4(%edi)
  add $0x8,        %edi
  add $0x40000000, %eax
  jnc 1f
  inc %edx
1:
  loop 0b

  mov $pml4, %ecx
  mov %ecx, %cr3

  mov %cr4, %ecx
  or $0x30, %ecx
  mov %ecx, %cr4

  mov $0xC0000080, %ecx
  rdmsr
  or $0x100, %eax
  wrmsr

  mov %cr0, %ecx
  or $0x80000000, %ecx
  mov %ecx, %cr0

  /***************************
   * Initialize Segmentation *
   ***************************/
  lgdt gdt_ptr
  ljmp $0x08, $0f
0:

.code64
  /****************
   * Getting to C *
   ****************/
  xor %ebp, %ebp
  mov $stack_end, %esp
  mov %ebx, %edi
  call bmain

0:
  hlt
  jmp 0b

