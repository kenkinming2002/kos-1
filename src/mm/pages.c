#include "pages.h"

#include "bitmap.h"

#include "core/assert.h"
#include "core/string.h"

#include <limits.h>

#define UINT_BIT (CHAR_BIT * sizeof(unsigned))
#define PAGE_SIZE 4096
#define PAGE_COUNT 1024

__attribute__((aligned(PAGE_SIZE))) struct page
{
  char data[PAGE_SIZE];
};

struct arena
{
  struct page pages[PAGE_COUNT];
  struct bitmap bm;

  unsigned bits[PAGE_COUNT / UINT_BIT];
};

struct arena arena;

void mm_init_pages_allocator()
{
  arena.bm.size = PAGE_COUNT;
  arena.bm.bits = arena.bits;
  bm_fill(arena.bm, 0, arena.bm.size, false);
}

struct page *_alloc_pages(unsigned count)
{
  size_t offset = bm_search(arena.bm, count, false);
  if(offset == BM_SEARCH_NONE)
    return NULL;

  bm_fill(arena.bm, offset, count, true);
  return &arena.pages[offset];
}

static void _free_pages(struct page *pages, unsigned count)
{
  size_t offset = pages - arena.pages;
  bm_fill(arena.bm, offset, count, false);
}

void *alloc_pages(unsigned count)
{
  return _alloc_pages(count);
}

void free_pages(void *pages, unsigned count)
{
  _free_pages(pages, count);
}

