#include "dev.h"

void dev_init()
{
  exceptions_init();
  i8259_init();
  pit_init();
}
