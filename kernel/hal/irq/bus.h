#ifndef HAL_IRQ_BUS_H
#define HAL_IRQ_BUS_H

#include <core/slot.h>

void irq_bus_add(const char *name, unsigned size);
void irq_bus_del(const char *name);

void irq_bus_set_input(const char *name, unsigned n, struct slot *slot);
void irq_bus_set_output(const char *name, unsigned n, struct slot *slot);

void irq_bus_unset_input(const char *name, unsigned n);
void irq_bus_unset_output(const char *name, unsigned n);

void irq_bus_init();

#endif // HAL_IRQ_BUS_H
