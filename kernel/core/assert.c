#include "assert.h"

#include "print.h"

void kassert(const char *file, int line)
{
  debug_printf("assertion failure file: %s, line: %d\n", file, line);
  for(;;) asm volatile("hlt");
}
