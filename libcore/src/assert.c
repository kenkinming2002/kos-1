#include <core/assert.h>

#include <core/log.h>

void kassert(const char *file, int line)
{
  logf("assertion failure file: {:s}, line: {:}\n", file, line);
  for(;;) asm volatile("hlt");
}
