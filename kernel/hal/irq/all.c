#include "all.h"

#include "hal/all.h"

#include <core/assert.h>

#include <stddef.h>
#include <stdint.h>

DEFINE_MODULE(irq_root);

static struct irq_slot irqs_slot[256];
void irq_init()
{
  irq_bus_init();

  KASSERT(res_acquire(RES_IRQ_BUS_ROOT_INPUT, THIS_MODULE, 0, 256) == 0);
  for(unsigned i=0; i<256; ++i)
    irq_bus_set_input(IRQ_BUS_ROOT, i, &irqs_slot[i]);
}

void isr(uint64_t irq, uint64_t /*ec*/)
{
  irq_slot_emit(&irqs_slot[irq]);
}


