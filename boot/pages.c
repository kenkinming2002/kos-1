#include "pages.h"

#include "mmap.h"
#include "modules.h"

#include "core/debug.h"

#include "page/region.h"
#include "page/watermark.h"

struct region usable_region;
struct region used_region;

struct watermark_allocator allocator;

void pages_init()
{
  for(size_t i=0; i<mmap_entry_count; ++i)
    if(mmap_entries[i].type == MEMORY_AVAILABLE)
      region_append(&usable_region, (struct area){mmap_entries[i].addr, mmap_entries[i].length});

  for(size_t i=0; i<module_count; ++i)
    region_append(&used_region, (struct area){modules[i].addr, modules[i].length});

  debug_printf("=====\n");

  for(size_t i=0; i<usable_region.count; ++i)
    debug_printf(" usable => addr=0x%lx, length=0x%lx\n",
        usable_region.areas[i].addr,
        usable_region.areas[i].length);

  for(size_t i=0; i<used_region.count; ++i)
    debug_printf(" used => addr=0x%lx, length=0x%lx\n",
        used_region.areas[i].addr,
        used_region.areas[i].length);

  debug_printf("=====\n");

  regions_intersect(&usable_region, &used_region);

  for(size_t i=0; i<usable_region.count; ++i)
    debug_printf(" usable => addr=0x%lx, length=0x%lx\n",
        usable_region.areas[i].addr,
        usable_region.areas[i].length);

  for(size_t i=0; i<used_region.count; ++i)
    debug_printf(" used => addr=0x%lx, length=0x%lx\n",
        used_region.areas[i].addr,
        used_region.areas[i].length);

  debug_printf("=====\n");

  watermark_initialize(&allocator, usable_region);
  for(size_t i=0; i<1024; ++i)
  {
    void *page = watermark_allocate(&allocator);
    debug_printf("allocated single page addr = 0x%lx\n", (uintptr_t)page);
  }
}

void *alloc_pages(size_t count)
{
}
