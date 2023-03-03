#ifndef HAL_IRQ_BUS_H
#define HAL_IRQ_BUS_H

#include "slot.h"

enum irq_bus_tag
{
  IRQ_BUS_ROOT,
  IRQ_BUS_EXCEPTIONS,
  IRQ_BUS_ISA,
  IRQ_BUS_GSI,
};

__attribute__((warn_unused_result)) int irq_bus_set_input(enum irq_bus_tag tag, unsigned n, struct irq_slot *slot);
__attribute__((warn_unused_result)) int irq_bus_set_output(enum irq_bus_tag tag, unsigned n, struct irq_slot *slot);

void irq_bus_unset_input(enum irq_bus_tag tag, unsigned n);
void irq_bus_unset_output(enum irq_bus_tag tag, unsigned n);

#endif // HAL_IRQ_BUS_H
