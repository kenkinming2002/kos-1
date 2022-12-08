#ifndef HAL_IRQS_H
#define HAL_IRQS_H

#include "module.h"

#include <core/ll.h>
#include <core/slot.h>

int acquire_irqs(struct module *module, unsigned begin, unsigned count);
int release_irqs(struct module *module, unsigned begin, unsigned count);

void irqs_init();

#endif // HAL_IRQS_H
