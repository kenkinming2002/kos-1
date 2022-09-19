#ifndef MM_BITMAP_H
#define MM_BITMAP_H

#include <stddef.h>
#include <stdbool.h>

struct bitmap
{
  size_t size;
  unsigned *bits;
};

void bm_set(struct bitmap bm, size_t i, bool value);
bool bm_get(struct bitmap bm, size_t i);

void bm_fill(struct bitmap bm, size_t offset, size_t count, bool value);
bool bm_test(struct bitmap bm, size_t offset, size_t count, bool value);

#define BM_SEARCH_NONE ((size_t)-1)
size_t bm_search(struct bitmap bm, size_t count, bool value);

#endif // MM_BITMAP_H
