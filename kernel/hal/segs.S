.global reload_segs

.section .data
target:
  .quad end
  .word 0x8

.section .text
reload_segs:
  mov $0x10, %eax
  mov %eax, %ss
  mov %eax, %ds
  mov %eax, %fs
  mov %eax, %gs

  lea target(%rip), %rax
  rex.w ljmp *(%rax) /* GAS does not accept ljmpq */
end:
  ret
