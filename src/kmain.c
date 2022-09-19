#include "debug.h"
#include "core/print.h"

#include "mm/mapping.h"
#include "mm/pages.h"

#include <stdint.h>

#define TEST_COUNT 10

void kmain()
{
  debug_init();
  debug_printf("hello\n");

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

  mm_init_kernel_mapping();
}
