#include "root.h"

#include "hal/all.h"

#include <core/assert.h>

DEFINE_MODULE(irq_root);

static struct irq_slot irq_root_slots[256];
void irq_root_init()
{
  KASSERT(res_acquire(RES_IRQ_BUS_ROOT_INPUT, THIS_MODULE, 0, 256) == 0);
  for(unsigned i=0; i<256; ++i)
    irq_bus_set_input(IRQ_BUS_ROOT, i, &irq_root_slots[i]);
}

void isr(uint64_t irq, uint64_t /*ec*/)
{
  irq_slot_emit(&irq_root_slots[irq]);
}


