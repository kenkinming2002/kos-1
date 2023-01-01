#include "bus.h"

#include "mm/all.h"

#include <kcore/assert.h>
#include <kcore/ll.h>
#include <kcore/string.h>

struct irq_bus_connection
{
  struct irq_slot *input;
  struct irq_slot *output;
};

struct irq_bus
{
  unsigned                   size;
  struct irq_bus_connection *conns;
};

#define DEFINE_IRQ_BUS(name, size) \
  struct irq_bus_connection name##_irq_bus_connections[size]; \
  struct irq_bus name##_irq_bus = { size, name##_irq_bus_connections };

DEFINE_IRQ_BUS(root,       256)
DEFINE_IRQ_BUS(exceptions, 32)
DEFINE_IRQ_BUS(isa,        16)
DEFINE_IRQ_BUS(gsi,        0)

static struct irq_bus *irq_bus_get(enum irq_bus_tag tag)
{
  switch(tag)
  {
  case IRQ_BUS_ROOT:       return &root_irq_bus;
  case IRQ_BUS_EXCEPTIONS: return &exceptions_irq_bus;
  case IRQ_BUS_ISA:        return &isa_irq_bus;
  case IRQ_BUS_GSI:        return &gsi_irq_bus;
  default:
    KASSERT_UNREACHABLE;
  }
}

static struct irq_bus_connection *irq_bus_connection_get(enum irq_bus_tag tag, unsigned n)
{
  struct irq_bus *bus = irq_bus_get(tag);
  KASSERT(n < bus->size);
  return &bus->conns[n];
}

int irq_bus_set_input(enum irq_bus_tag tag, unsigned n, struct irq_slot *slot)
{
  struct irq_bus_connection *conn = irq_bus_connection_get(tag, n);
  if(conn->input)
    return -1;

  conn->input = slot;
  if(conn->output)
    irq_slot_connect(conn->input, conn->output);

  return 0;
}

int irq_bus_set_output(enum irq_bus_tag tag, unsigned n, struct irq_slot *slot)
{
  struct irq_bus_connection *conn = irq_bus_connection_get(tag, n);
  if(conn->output)
    return -1;

  conn->output = slot;
  if(conn->input)
    irq_slot_connect(conn->input, conn->output);

  return 0;
}

void irq_bus_unset_input(enum irq_bus_tag tag, unsigned n)
{
  struct irq_bus_connection *conn = irq_bus_connection_get(tag, n);
  KASSERT(conn->input);
  if(conn->output)
    irq_slot_disconnect(conn->input, conn->output);
  conn->input = NULL;
}

void irq_bus_unset_output(enum irq_bus_tag tag, unsigned n)
{
  struct irq_bus_connection *conn = irq_bus_connection_get(tag, n);
  KASSERT(conn->output);
  if(conn->input)
    irq_slot_disconnect(conn->input, conn->output);
  conn->output = NULL;
}

void irq_bus_init()
{
  // This may be needed in the future since the GSI bus would likely be
  // dynamically sized, so keep it for now.
}

