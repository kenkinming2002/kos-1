#include "pal.h"

void pal_init()
{
  gdt_init();
  idt_init();
}
