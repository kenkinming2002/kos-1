#include "exceptions.h"

#include "hal.h"
#include "mm.h"

#include <core/assert.h>

DEFINE_MODULE(exceptions);

static struct slot exceptions_slot[32];
void exceptions_module_init()
{
  KASSERT(acquire_irqs(THIS_MODULE, 0, 32) == 0);
  for(unsigned i=0; i<32; ++i)
  {
    irq_bus_set_output("root",       i, &exceptions_slot[i]);
    irq_bus_set_input ("exceptions", i, &exceptions_slot[i]);
  }
}

