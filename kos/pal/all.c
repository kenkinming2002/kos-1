#include "all.h"

void pal_init()
{
  gdt_init();
  idt_init();
}

void pal_load()
{
  gdt_load();
  idt_load();
}
