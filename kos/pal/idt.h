#ifndef PAL_IDT_H
#define PAL_IDT_H

#define IDT_MAX_VECTOR     ((unsigned)256)
#define IDT_INVALID_VECTOR ((unsigned)-1)

void idt_init();
void idt_load();

#endif // PAL_IDT_H
