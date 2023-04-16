#include "memory.h"
#include "pages.h"

void kmain(struct kboot_info *boot_info)
{
  memory_init(boot_info);
  pages_init(boot_info);
  for(;;) asm volatile("hlt");
}

