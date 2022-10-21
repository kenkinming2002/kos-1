#include "core/assert.h"

#include "core/print.h"

void kassert(const char *file, int line)
{
  debug_printf("assertion failure file: %s, line: %d\n", file, line);
  for(;;) asm volatile("hlt");
}
