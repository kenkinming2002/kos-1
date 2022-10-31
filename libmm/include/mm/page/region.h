#ifndef PAGE_REGION_H
#define PAGE_REGION_H

#include <stddef.h>
#include <stdint.h>

#define MAX_AREAS 32

struct area
{
  uintptr_t addr;
  size_t    length;
};

struct region
{
  struct area areas[MAX_AREAS];
  size_t      count;
};

void region_append(struct region *region, struct area area);

/*
 * Intersect 2 regions, modifying it so that only non-intersection region is
 * left.
 */
void regions_intersect(struct region *region1, struct region *region2);

#endif // PAGE_REGION_H
