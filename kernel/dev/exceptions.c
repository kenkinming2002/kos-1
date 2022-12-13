#include "exceptions.h"

#include "hal/all.h"
#include "mm/all.h"

#include <core/assert.h>

DEFINE_MODULE(exceptions);

static struct irq_slot exceptions_slot[32];
void exceptions_module_init()
{
  KASSERT(res_acquire(RES_IRQ_BUS_ROOT_OUTPUT, THIS_MODULE, 0, 32) == 0);
  for(unsigned i=0; i<32; ++i)
  {
    irq_bus_set_output(IRQ_BUS_ROOT,       i, &exceptions_slot[i]);
    irq_bus_set_input (IRQ_BUS_EXCEPTIONS, i, &exceptions_slot[i]);
  }
}

