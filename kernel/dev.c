#include "dev.h"

void dev_init()
{
  exceptions_init();
  pic8259_init();
  pit_init();
}
