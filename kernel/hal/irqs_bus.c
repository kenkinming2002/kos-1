#include "irqs_bus.h"

#include "mm.h"

#include <core/assert.h>
#include <core/ll.h>
#include <core/string.h>

struct irqs_bus
{
  struct ll_node node;

  const char *name;
  unsigned size;
  struct slot **slots;
};
LL_DEFINE(irqs_bus_list);

void irqs_bus_add(const char *name, unsigned size)
{
  struct irqs_bus *irqs_bus = kmalloc(sizeof *irqs_bus);
  irqs_bus->name = name;
  irqs_bus->size = size;
  irqs_bus->slots = kmalloc(sizeof *irqs_bus->slots * size);
  ll_append(&irqs_bus_list, &irqs_bus->node);
}

void irqs_bus_del(const char *name)
{
  LL_FOREACH(irqs_bus_list, node)
  {
    struct irqs_bus *irqs_bus = (struct irqs_bus *)node;
    if(strcmp(name, irqs_bus->name) != 0)
      continue;

    ll_delete(node);
    kfree(irqs_bus->slots);
    kfree(irqs_bus);
    return;
  }
  KASSERT_UNREACHABLE;
}

void irqs_bus_set(const char *name, unsigned n, struct slot *slot)
{
  LL_FOREACH(irqs_bus_list, node)
  {
    struct irqs_bus *irqs_bus = (struct irqs_bus *)node;
    if(strcmp(name, irqs_bus->name) != 0)
      continue;

    KASSERT(n<irqs_bus->size);
    irqs_bus->slots[n] = slot;
    return;
  }
  KASSERT_UNREACHABLE;
}

struct slot *irqs_bus_get(const char *name, unsigned n)
{
  LL_FOREACH(irqs_bus_list, node)
  {
    struct irqs_bus *irqs_bus = (struct irqs_bus *)node;
    if(strcmp(name, irqs_bus->name) != 0)
      continue;

    KASSERT(n<irqs_bus->size);
    return irqs_bus->slots[n];
  }
  KASSERT_UNREACHABLE;
}

