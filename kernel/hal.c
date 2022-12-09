#include "hal.h"

void hal_init()
{
  irqs_bus_init();
  irqs_init();
}
