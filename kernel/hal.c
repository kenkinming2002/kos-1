#include "hal.h"

void hal_init()
{
  gdt_init();
  idt_init();
  exceptions_init();
}
