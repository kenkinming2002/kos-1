#ifndef PIC_ISA_H
#define PIC_ISA_H

#include "hal.h"

int isa_irq_register(struct module *module, unsigned irq, handler_t handler);
int isa_irq_deregister(struct module *module, unsigned irq, handler_t handler);

#endif // PIC_ISA_H
