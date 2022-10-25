#include "page/watermark.h"

void *watermark_initialize(struct watermark_allocator *allocator, struct region region)
{
  allocator->region = region;
  for(size_t i=0; i<allocator->region.count; ++i)
  {
    struct area *area = &allocator->region.areas[i];
    if(allocator->max <= area->addr + area->length)
      allocator->max = area->addr + area->length;
  }
}

void *watermark_allocate(struct watermark_allocator *allocator)
{
  for(;allocator->cur < allocator->max; allocator->cur += 0x1000)
    for(size_t i=0; i<allocator->region.count; ++i)
    {
      struct area *area = &allocator->region.areas[i];
      if(area->addr <= allocator->cur && allocator->cur + 0x1000 <= area->addr + area->length)
      {
        void *addr = (void*)allocator->cur;
        allocator->cur += 0x1000;
        return addr;
      }
    }
}
