#define STACK_SIZE 0x1000

.global entry
.extern kmain

/* Higher half code */
.section .bss
  initial_stack: .fill STACK_SIZE, 1, 0

.section .text
  entry:
    xor %ebp, %ebp
    mov $(initial_stack + STACK_SIZE), %esp

    push %ebx
    push %eax
    call kmain
  0:
    hlt
    jmp 0b
