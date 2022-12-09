#include "irqs_bus.h"

#include "mm.h"

#include <core/assert.h>
#include <core/ll.h>
#include <core/string.h>

struct irqs_bus_connection
{
  struct slot *input;
  struct slot *output;
};

struct irqs_bus
{
  struct ll_node node;

  const char                 *name;
  unsigned                    size;
  struct irqs_bus_connection *connections;
};
LL_DEFINE(irqs_bus_list);

static struct irqs_bus *irqs_bus_lookup(const char *name)
{
  LL_FOREACH(irqs_bus_list, node)
  {
    struct irqs_bus *bus = (struct irqs_bus *)node;
    if(strcmp(name, bus->name) != 0)
      continue;

    return bus;
  }
  return NULL;
}

void irqs_bus_add(const char *name, unsigned size)
{
  struct irqs_bus *bus = irqs_bus_lookup(name);
  KASSERT(!bus);
  bus = kmalloc(sizeof *bus);
  bus->name = name;
  bus->size = size;
  bus->connections = kmalloc(sizeof *bus->connections * size);
  ll_append(&irqs_bus_list, &bus->node);
}

void irqs_bus_del(const char *name)
{
  struct irqs_bus *bus = irqs_bus_lookup(name);
  KASSERT(bus);
  ll_delete(&bus->node);
  kfree(bus->connections);
  kfree(bus);
  return;
}

void irqs_bus_set_input(const char *name, unsigned n, struct slot *slot)
{
  struct irqs_bus *bus = irqs_bus_lookup(name);
  KASSERT(bus);
  KASSERT(n<bus->size);

  struct irqs_bus_connection *conn = &bus->connections[n];
  KASSERT(!conn->input);
  conn->input = slot;
  if(conn->output)
    slot_connect(conn->input, conn->output);
}

void irqs_bus_set_output(const char *name, unsigned n, struct slot *slot)
{
  struct irqs_bus *bus = irqs_bus_lookup(name);
  KASSERT(bus);
  KASSERT(n<bus->size);

  struct irqs_bus_connection *conn = &bus->connections[n];
  KASSERT(!conn->output);
  conn->output = slot;
  if(conn->input)
    slot_connect(conn->input, conn->output);
}

void irqs_bus_unset_input(const char *name, unsigned n)
{
  struct irqs_bus *bus = irqs_bus_lookup(name);
  KASSERT(bus);
  KASSERT(n<bus->size);

  struct irqs_bus_connection *conn = &bus->connections[n];
  KASSERT(conn->input);
  if(conn->output)
    slot_disconnect(conn->input, conn->output);
  conn->input = NULL;
}

void irqs_bus_unset_output(const char *name, unsigned n)
{
  struct irqs_bus *bus = irqs_bus_lookup(name);
  KASSERT(bus);
  KASSERT(n<bus->size);

  struct irqs_bus_connection *conn = &bus->connections[n];
  KASSERT(conn->output);
  if(conn->input)
    slot_disconnect(conn->input, conn->output);
  conn->output = NULL;
}

