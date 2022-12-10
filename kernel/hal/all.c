#include "all.h"

void hal_init()
{
  irq_bus_init();
  irqs_init();
}
