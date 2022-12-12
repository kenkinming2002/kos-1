#include "all.h"

#include <stddef.h>
#include <stdint.h>

static struct irq_slot irqs_slot[256];
void irq_init()
{
  irq_bus_init();
  for(unsigned i=0; i<256; ++i)
    irq_bus_set_input(IRQ_BUS_ROOT, i, &irqs_slot[i]);
}

void isr(uint64_t irq, uint64_t /*ec*/)
{
  irq_slot_emit(&irqs_slot[irq]);
}


