#include "bitmap.h"

#include <limits.h>

void bm_set(struct bitmap bm, size_t i, bool value)
{
  const size_t index = i / UINT_BIT;
  const size_t shift = i % UINT_BIT;
  bm.bits[index] &= ~(1 << shift);
  bm.bits[index] |= value << shift;
}

bool bm_get(struct bitmap bm, size_t i)
{
  const size_t index = i / UINT_BIT;
  const size_t shift = i % UINT_BIT;
  return bm.bits[index] & (1 << shift);
}

void bm_fill(struct bitmap bm, size_t offset, size_t count, bool value)
{
  for(size_t i=offset; i<offset+count; ++i)
    bm_set(bm, i, value);
}

bool bm_test(struct bitmap bm, size_t offset, size_t count, bool value)
{
  for(size_t i=offset; i<offset+count; ++i)
    if(bm_get(bm, i) != value)
      return false;

  return true;
}

size_t bm_search(struct bitmap bm, size_t count, bool value)
{
  for(size_t offset=0; offset+count<=bm.size; ++offset)
    if(bm_test(bm, offset, count, value))
      return offset;

  return BM_SEARCH_NONE;
}

