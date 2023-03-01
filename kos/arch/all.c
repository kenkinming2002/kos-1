#include "all.h"

void arch_init()
{
  gdt_init();
  idt_init();
}

void arch_load()
{
  gdt_load();
  idt_load();
}
