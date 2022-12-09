#ifndef HAL_IRQS_BUS_H
#define HAL_IRQS_BUS_H

#include <core/slot.h>

void irqs_bus_add(const char *name, unsigned size);
void irqs_bus_del(const char *name);

void irqs_bus_set_input(const char *name, unsigned n, struct slot *slot);
void irqs_bus_set_output(const char *name, unsigned n, struct slot *slot);

void irqs_bus_unset_input(const char *name, unsigned n);
void irqs_bus_unset_output(const char *name, unsigned n);

#endif // HAL_IRQS_BUS_H
