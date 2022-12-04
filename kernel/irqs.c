#include "irqs.h"

void irqs_init()
{
  exceptions_init();
  pic8259_init();
}
