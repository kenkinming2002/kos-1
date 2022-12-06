#ifndef ISA_H
#define ISA_H

#include "hal.h"

int isa_irq_register(struct module *module, unsigned irq, handler_t handler, void *data);
int isa_irq_deregister(struct module *module, unsigned irq);

#endif // ISA_H
