.global bsp_entry
.global ap_entry

.extern boot_info
.extern bmain

#define STACK_SIZE  0x1000
#define STACK_COUNT 16

/********************
 * Global Variables *
 ********************/
.section .system_structures, "aw", @nobits
  .align 0x1000
  stack:
    .fill STACK_SIZE * STACK_COUNT, 1, 0
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

.section .bss
boot_info:
  .long 0x0

/*******************
 * Entry point *
 *******************/
.section .text
bsp_entry:
.code32
  cmp $0x36d76289, %eax
  jne 0f

  mov %ebx, boot_info
  jmp entry

// Error: We are not boot by a multiboot2 compliant bootloader
0:
  hlt
  jmp 0b

ap_entry:
.code32
  mov boot_info, %ebx
  jmp entry

0:
  hlt
  jmp 0b

// ebx: address of multiboot2 information structure for BSP, 0 otherwise
entry:
.code32
  /**************************
   * BSP: Initialize Paging *
   **************************/
  mov $0x1B, %ecx
  rdmsr
  bt $0x8, %eax
  jnc 2f

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

2:

  /*********************
   * Initialize Paging *
   *********************/
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
  /******************
   * Initialize SMP *
   ******************/
  mov $0x1B, %ecx
  rdmsr
  bt $0x8, %eax
  jnc 1f

  // Retrieve APIC Base
  shl $0x20, %rdx
  or  %rax,  %rdx
  mov $0x0000000FFFFFF000, %rax // TODO: Determine the mask from MAXPHYADDR
  and %rax, %rdx

  /************
   * INIT SPI *
   ************/
  mov 0x300(%rdx), %eax
  and $0xFFF33000, %eax
  or  $0x000C4500, %eax
  mov %eax, 0x300(%rdx)

  // Wait for delivery
0:
  pause
  mov 0x300(%rdx), %eax
  bt $0xC, %eax
  jc 0b

  // Wait for 10ms
  mov $0b00110000, %al // Channel 0; Lobyte/Hibyte; Mode 0
  out %al, $0x43
  mov $0x9C, %al // Lobyte
  out %al, $0x40
  mov $0x2E, %al // Hibyte
  out %al, $0x40

0:
  mov 0b11100010, %al
  out %al, $0x43
  in $0x40, %al
  bt $0x7, %ax
  jnc 0b

  /***************
   * STARTUP SPI *
   ***************/
  mov 0x300(%rdx), %eax
  and $0xFFF33000, %eax
  or  $0x000C4608, %eax
  mov %eax, 0x300(%rdx)

  // Wait for delivery
0:
  pause
  mov 0x300(%rdx), %eax
  bt $0xC, %eax
  jc 0b

  // Wait for 200us
  mov $0b00110000, %al // Channel 0; Lobyte/Hibyte; Mode 0
  out %al, $0x43
  mov $0xEF, %al // Lobyte
  out %al, $0x40
  mov $0x00, %al // Hibyte
  out %al, $0x40

0:
  mov 0b11100010, %al
  out %al, $0x43
  in $0x40, %al
  bt $0x7,  %ax
  jnc 0b

1:

  /****************
   * Getting to C *
   ****************/
  xor %ebp, %ebp
  mov $(-STACK_SIZE), %rsp
  lock xaddl %esp, stack_ptr

  mov %ebx, %edi
  call bmain

0:
  hlt
  jmp 0b

