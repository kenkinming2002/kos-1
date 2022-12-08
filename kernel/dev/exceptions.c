#include "exceptions.h"

#include "hal.h"
#include "mm.h"

#include <core/assert.h>

DEFINE_MODULE(exceptions);

void exceptions_module_init()
{
  KASSERT(acquire_irqs(THIS_MODULE, 0, 32) == 0);
  irqs_bus_add("exceptions", 32);
  for(unsigned i=0; i<32; ++i)
    irqs_bus_set("exceptions", i, irqs_bus_get("root", i));
}

