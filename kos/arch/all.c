#include "all.h"

#include <arch/once.h>

void arch_init()
{
  static struct once once;
  if(once_begin(&once, ONCE_SYNC))
  {
    gdt_init();
    idt_init();

    once_end(&once, ONCE_SYNC);
  }

  gdt_load();
  idt_load();
}

