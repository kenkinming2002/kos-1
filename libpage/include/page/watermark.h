#ifndef PAGE_WATERMARK_H
#define PAGE_WATERMARK_H

/* Watermark allocator */

#include "region.h"

struct watermark_allocator
{
  struct region region;
  uintptr_t cur, max;
};

void *watermark_initialize(struct watermark_allocator *allocator, struct region region);
void *watermark_allocate(struct watermark_allocator *allocator);

#endif // PAGE_WATERMARK_H
