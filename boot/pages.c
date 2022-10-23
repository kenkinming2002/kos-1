#include "pages.h"

#include "mmap.h"
#include "modules.h"

#include "core/assert.h"
#include "core/string.h"
#include "core/debug.h"

#include <stdbool.h>

#define MAX_MEM_AREAS 32

struct mem_area
{
  uintptr_t addr;
  size_t    length;
};

struct mem_region
{
  struct mem_area areas[MAX_MEM_AREAS];
  size_t          count;
};

static void region_append(struct mem_region *region, struct mem_area area)
{
  KASSERT(region->count != MAX_MEM_AREAS);
  region->areas[region->count++] = area;
}

static void regions_intersect(struct mem_region *region1, struct mem_region *region2)
{
  struct mem_region new_region1 = {0};
  struct mem_region new_region2 = {0};

  size_t i=0, j=0;
  while(i<region1->count && j<region2->count)
  {
    struct mem_area *area1 = &region1->areas[i];
    struct mem_area *area2 = &region2->areas[j];

    // If either area is completely in front of the other area, great.
    if(area1->addr + area1->length <= area2->addr)
    {
      region_append(&new_region1, *area1);
      ++i;
    }
    else if(area2->addr + area2->length <= area1->addr)
    {
      region_append(&new_region2, *area2);
      ++j;
    }
    else
    {
      // We have overlap here, chop off the excess part so that both areas have the same starting address
      if(area1->addr < area2->addr)
      {
        size_t chop = area2->addr - area1->addr;
        region_append(&new_region1, (struct mem_area){.addr = area1->addr, .length = chop});
        area1->addr   += chop;
        area1->length -= chop;
      }
      else if(area2->addr < area1->addr)
      {
        size_t chop = area1->addr - area2->addr;
        region_append(&new_region2, (struct mem_area){.addr = area2->addr, .length = chop});
        area2->addr   += chop;
        area2->length -= chop;
      }

      // At this point both leading area have same address compare size to see who wins
      if(area1->length < area2->length)
      {
        ++i;
        area2->addr   += area1->length;
        area2->length -= area1->length;
      }
      else if(area2->length < area1->length)
      {
        ++j;
        area1->addr   += area2->length;
        area1->length -= area2->length;
      }
    }
  }

  // Finish off the remaining areas
  for(; i<region1->count; ++i)
    region_append(&new_region1, region1->areas[i]);

  for(; i<region2->count; ++i)
    region_append(&new_region2, region2->areas[i]);

  // Copy back
  memcpy(region1, &new_region1, sizeof *region1);
  memcpy(region2, &new_region2, sizeof *region2);
}

struct mem_region usable_region;
struct mem_region used_region;

void pages_init()
{
  for(size_t i=0; i<mmap_entry_count; ++i)
    if(mmap_entries[i].type == MEMORY_AVAILABLE)
      region_append(&usable_region, (struct mem_area){mmap_entries[i].addr, mmap_entries[i].length});

  for(size_t i=0; i<module_count; ++i)
    region_append(&used_region, (struct mem_area){modules[i].addr, modules[i].length});

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
}

void *alloc_pages(size_t count)
{
}
