#ifndef ARCH_IDT_H
#define ARCH_IDT_H

#define IDT_MAX_VECTOR     ((unsigned)256)
#define IDT_INVALID_VECTOR ((unsigned)-1)

void idt_init();
void idt_load();

#endif // ARCH_IDT_H
