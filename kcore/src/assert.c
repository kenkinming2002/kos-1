#include <kcore/assert.h>

#include <kcore/debug.h>

void kassert(const char *file, int line)
{
  debug_printf("assertion failure file: %s, line: %d\n", file, line);
  for(;;) asm volatile("hlt");
}
