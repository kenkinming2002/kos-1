#include "dev.h"

void dev_init()
{
  exceptions_init();
  i8259_init();
  i8253_init();
}
