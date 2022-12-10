#include "bus.h"

#include "mm.h"

#include <core/assert.h>
#include <core/ll.h>
#include <core/string.h>

struct irq_bus_connection
{
  struct irq_slot *input;
  struct irq_slot *output;
};

struct irq_bus
{
  struct ll_node node;

  const char                 *name;
  unsigned                    size;
  struct irq_bus_connection *connections;
};
LL_DEFINE(irq_bus_list);

static struct irq_bus *irq_bus_lookup(const char *name)
{
  LL_FOREACH(irq_bus_list, node)
  {
    struct irq_bus *bus = (struct irq_bus *)node;
    if(strcmp(name, bus->name) != 0)
      continue;

    return bus;
  }
  return NULL;
}

void irq_bus_add(const char *name, unsigned size)
{
  struct irq_bus *bus = irq_bus_lookup(name);
  KASSERT(!bus);
  bus = kmalloc(sizeof *bus);
  bus->name = name;
  bus->size = size;
  bus->connections = kmalloc(sizeof *bus->connections * size);
  ll_append(&irq_bus_list, &bus->node);
}

void irq_bus_del(const char *name)
{
  struct irq_bus *bus = irq_bus_lookup(name);
  KASSERT(bus);
  ll_delete(&bus->node);
  kfree(bus->connections);
  kfree(bus);
  return;
}

void irq_bus_set_input(const char *name, unsigned n, struct irq_slot *slot)
{
  struct irq_bus *bus = irq_bus_lookup(name);
  KASSERT(bus);
  KASSERT(n<bus->size);

  struct irq_bus_connection *conn = &bus->connections[n];
  KASSERT(!conn->input);
  conn->input = slot;
  if(conn->output)
    irq_slot_connect(conn->input, conn->output);
}

void irq_bus_set_output(const char *name, unsigned n, struct irq_slot *slot)
{
  struct irq_bus *bus = irq_bus_lookup(name);
  KASSERT(bus);
  KASSERT(n<bus->size);

  struct irq_bus_connection *conn = &bus->connections[n];
  KASSERT(!conn->output);
  conn->output = slot;
  if(conn->input)
    irq_slot_connect(conn->input, conn->output);
}

void irq_bus_unset_input(const char *name, unsigned n)
{
  struct irq_bus *bus = irq_bus_lookup(name);
  KASSERT(bus);
  KASSERT(n<bus->size);

  struct irq_bus_connection *conn = &bus->connections[n];
  KASSERT(conn->input);
  if(conn->output)
    irq_slot_disconnect(conn->input, conn->output);
  conn->input = NULL;
}

void irq_bus_unset_output(const char *name, unsigned n)
{
  struct irq_bus *bus = irq_bus_lookup(name);
  KASSERT(bus);
  KASSERT(n<bus->size);

  struct irq_bus_connection *conn = &bus->connections[n];
  KASSERT(conn->output);
  if(conn->input)
    irq_slot_disconnect(conn->input, conn->output);
  conn->output = NULL;
}

void irq_bus_init()
{
  irq_bus_add("root",        256);
  irq_bus_add("isa",         16);
  irq_bus_add("exceptions",  32);
}

