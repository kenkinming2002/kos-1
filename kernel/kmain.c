#include "core/debug.h"
#include "debug.h"

#include <stdint.h>

#define TEST_COUNT 10

void kmain()
{
  debug_init();
  debug_printf("hello\n");
  for(;;) asm volatile("hlt");
}
