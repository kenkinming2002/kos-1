#include "assert.h"

void kassert(const char *file, int line)
{
  for(;;) asm volatile("hlt");
}
