#ifndef PIC_PIC8259_H
#define PIC_PIC8259_H

#include "hal.h"

void pic8259s_init();
int pic8259s_irq_register(struct module *module, unsigned irq, handler_t handler);
int pic8259s_irq_deregister(struct module *module, unsigned irq, handler_t handler);

void pic8259s_irq_acknowledge(unsigned irq);
void pic8259s_irq_mask(unsigned irq);
void pic8259s_irq_unmask(unsigned irq);

#endif // PIC_PIC8259_H
