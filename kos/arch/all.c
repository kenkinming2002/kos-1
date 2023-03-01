#include "all.h"

#include <arch/once.h>

void arch_init()
{
  static struct once once;
  if(once_begin(&once, ONCE_SYNC))
  {
    segmentation_init();
    interrupt_init();

    once_end(&once, ONCE_SYNC);
  }

  segmentation_load();
  interrupt_load();
}

