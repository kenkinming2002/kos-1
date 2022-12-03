#ifndef HAL_IDT_H
#define HAL_IDT_H

#include "module.h"

#define IDT_MAX_VECTOR     ((unsigned)256)
#define IDT_INVALID_VECTOR ((unsigned)-1)

void idt_init();

int idt_acquire_range(struct module *module, unsigned base, unsigned count);
int idt_release_range(struct module *module, unsigned base, unsigned count);

typedef void(*handler_t)();
int idt_register(struct module *module, unsigned vector, handler_t handler);
int idt_deregister(struct module *module, unsigned vector, handler_t handler);

unsigned idt_alloc_range(struct module *module, unsigned count);
void idt_free_range(struct module *module, unsigned base, unsigned count);


#endif // HAL_IDT_H
