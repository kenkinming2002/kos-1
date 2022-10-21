#include "core/print.h"

#include "boot_params.h"
#include "debug.h"

#include "mm/pages.h"

#include <stdint.h>

#define TEST_COUNT 10

void kmain(uint32_t magic, uint32_t addr)
{
  debug_init();
  debug_printf("hello\n");
  boot_params_init(magic, addr);

  mm_init_pages_allocator();

  void *pages[TEST_COUNT];

  for(int i=0; i<TEST_COUNT; ++i)
    pages[i] = alloc_pages(i+1);

  for(int i=0; i<TEST_COUNT; ++i)
    debug_printf("pages i=%d, addr=0x%lx\n", i, (uintptr_t)pages[i]);

  for(int i=0; i<TEST_COUNT; ++i)
    free_pages(pages[i], i+1);

  for(int i=0; i<TEST_COUNT; ++i)
    pages[i] = alloc_pages(TEST_COUNT-i);

  for(int i=0; i<TEST_COUNT; ++i)
    debug_printf("pages i=%d, addr=0x%lx\n", i, (uintptr_t)pages[i]);

  for(int i=0; i<TEST_COUNT; ++i)
    free_pages(pages[i], TEST_COUNT-i);

  for(;;) asm volatile("hlt");
}
