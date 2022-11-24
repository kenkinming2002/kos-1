#include "core/debug.h"

#include "debug.h"
#include "mm/pages.h"

#include <boot/service.h>

#include <stdint.h>

#define TEST_COUNT 10

void kmain(struct boot_service *service)
{
  debug_init();
  debug_printf("hello\n");

  mm_init_pages(service);
  for(;;) asm volatile("hlt");
}
