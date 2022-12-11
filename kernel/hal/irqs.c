#include "irqs.h"

#include "irq/bus.h"
#include "irq/slot.h"

#include <stddef.h>
#include <stdint.h>

static struct irq_slot irqs_slot[256];
void irqs_init()
{
  for(unsigned i=0; i<256; ++i)
    irq_bus_set_input(IRQ_BUS_ROOT, i, &irqs_slot[i]);
}

void isr(uint64_t irq, uint64_t ec)
{
  irq_slot_emit_forward(&irqs_slot[irq]);
}

