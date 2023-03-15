#ifndef HAL_IRQ_H
#define HAL_IRQ_H

#include <core/slot.h>

enum irq_bus_tag
{
  IRQ_BUS_ROOT,
  IRQ_BUS_EXCEPTION,
  IRQ_BUS_ISA,
  IRQ_BUS_GSI,
};

int irq_init();
struct slot *irq_slot(enum irq_bus_tag tag, unsigned n);

#endif // HAL_IRQ_H
