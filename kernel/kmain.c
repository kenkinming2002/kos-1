#include "core/debug.h"
#include "debug.h"

#include <stdint.h>

#define TEST_COUNT 10

void kmain(uint32_t magic, uint32_t addr)
{
  debug_init();
  debug_printf("hello\n");
  for(;;) asm volatile("hlt");
}
