#ifndef HAL_IRQS_H
#define HAL_IRQS_H

#include "driver.h"

typedef void(*irq_handler_t)();

/* Note: irq refers to the irq number as in the pic without any offsets */
int acquire_irq(struct driver *driver, unsigned irq, irq_handler_t handler);
int release_irq(struct driver *driver, unsigned irq, irq_handler_t handler);

int trigger_irq(unsigned irq);

#endif // HAL_IRQS_H
