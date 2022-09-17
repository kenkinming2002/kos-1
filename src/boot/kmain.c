#include "debug.h"
#include "core/print.h"

#include "mm/mapping.h"

void kmain()
{
  debug_init();
  debug_printf("hello\n");
  mm_init_kernel_mapping();
}
