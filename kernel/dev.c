#include "dev.h"

void dev_init()
{
  exceptions_module_init();
  i8259_module_init();
  i8253_module_init();
}
