#ifndef IRQS_ISA_H
#define IRQS_ISA_H

#include "hal.h"

int isa_irq_register(struct module *module, unsigned irq, handler_t handler, void *data);
int isa_irq_deregister(struct module *module, unsigned irq);

#endif // IRQS_ISA_H
