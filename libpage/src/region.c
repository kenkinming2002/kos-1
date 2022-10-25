#include "page/region.h"

#include "core/assert.h"
#include "core/string.h"

void region_append(struct region *region, struct area area)
{
  KASSERT(region->count != MAX_AREAS);
  region->areas[region->count++] = area;
}

void regions_intersect(struct region *region1, struct region *region2)
{
  struct region new_region1 = {0};
  struct region new_region2 = {0};

  size_t i=0, j=0;
  while(i<region1->count && j<region2->count)
  {
    struct area *area1 = &region1->areas[i];
    struct area *area2 = &region2->areas[j];

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
        region_append(&new_region1, (struct area){.addr = area1->addr, .length = chop});
        area1->addr   += chop;
        area1->length -= chop;
      }
      else if(area2->addr < area1->addr)
      {
        size_t chop = area1->addr - area2->addr;
        region_append(&new_region2, (struct area){.addr = area2->addr, .length = chop});
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
