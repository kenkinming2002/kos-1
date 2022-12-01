#ifndef HAL_IRQS_H
#define HAL_IRQS_H

#include "module.h"

typedef unsigned irq_t;
typedef void(*irq_handler_t)();

int acquire_irq(struct module *module, irq_t irq, irq_handler_t handler);
int release_irq(struct module *module, irq_t irq, irq_handler_t handler);
int trigger_irq(irq_t irq);

#endif // HAL_IRQS_H
