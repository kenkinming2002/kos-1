#ifndef PIC_ISA_H
#define PIC_ISA_H

#include "hal.h"

typedef void(*handler_t)();

int isa_irq_register(struct module *module, unsigned irq, handler_t handler);
int isa_irq_deregister(struct module *module, unsigned irq, handler_t handler);

void isa_irq_acknowledge(unsigned irq);
void isa_irq_mask(unsigned irq);
void isa_irq_unmask(unsigned irq);

#endif // PIC_ISA_H
