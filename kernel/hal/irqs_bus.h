#ifndef HAL_IRQS_BUS_H
#define HAL_IRQS_BUS_H

#include <core/slot.h>

void irqs_bus_add(const char *name, unsigned size);
void irqs_bus_del(const char *name);

void irqs_bus_set(const char *name, unsigned n, struct slot *slot);
struct slot *irqs_bus_get(const char *name, unsigned n);

#endif // HAL_IRQS_BUS_H
