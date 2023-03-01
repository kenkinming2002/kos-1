.global segmentation_load
.extern gdt_desc

.section .data
target:
  .quad end
  .word 0x8

.section .text
segmentation_load:
  lgdt gdt_desc(%rip)
  mov $0x10, %eax
  mov %eax, %ss
  mov %eax, %ds
  mov %eax, %fs
  mov %eax, %gs

  lea target(%rip), %rax
  rex.w ljmp *(%rax) /* GAS does not accept ljmpq */
end:
  ret
