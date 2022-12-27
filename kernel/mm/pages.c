#include "pages.h"

#include <core/assert.h>
#include <core/bitmap.h>

#define PAGE_SIZE 0x1000

static struct bitmap bm;

#define ALIGN_UP(value, align)   ((value + align - 1) / align * align)
#define ALIGN_DOWN(value, align) (value / align * align)

static void *mm_alloc_initial(struct kboot_info *boot_info, size_t size)
{
  for(size_t i=0; i<boot_info->mmap->count; ++i)
    if(boot_info->mmap->entries[i].type == KBOOT_CONVENTIONAL_MEMORY)
      if(boot_info->mmap->entries[i].end - boot_info->mmap->entries[i].begin >= size)
        return (void*)boot_info->mmap->entries[i].begin;

  KASSERT_UNREACHABLE;
}

void mm_init_pages(struct kboot_info *boot_info)
{
  uint64_t max_addr = 0;
  for(size_t i=0; i<boot_info->mmap->count; ++i)
    if(boot_info->mmap->entries[i].type == KBOOT_CONVENTIONAL_MEMORY)
      if(max_addr < boot_info->mmap->entries[i].end)
        max_addr = boot_info->mmap->entries[i].end;

  size_t page_count  = (max_addr   + PAGE_SIZE - 1) / PAGE_SIZE;
  size_t bitmap_size = (page_count + UINT_BIT  - 1) / UINT_BIT;
  bm.size = page_count;
  bm.bits = mm_alloc_initial(boot_info, bitmap_size * sizeof *bm.bits);

  bm_fill(bm, 0, bm.size, true);
  for(size_t i=0; i<boot_info->mmap->count; ++i)
    if(boot_info->mmap->entries[i].type == KBOOT_CONVENTIONAL_MEMORY)
    {
      size_t begin_index = ALIGN_UP  (boot_info->mmap->entries[i].begin, PAGE_SIZE) / PAGE_SIZE;
      size_t end_index   = ALIGN_DOWN(boot_info->mmap->entries[i].end,   PAGE_SIZE) / PAGE_SIZE;
      if(begin_index > end_index)
        continue;

      bm_fill(bm, begin_index, end_index - begin_index, false);
    }

  size_t begin_index = ALIGN_DOWN((uintptr_t)(bm.bits),               PAGE_SIZE) / PAGE_SIZE;
  size_t end_index   = ALIGN_UP  ((uintptr_t)(bm.bits + bitmap_size), PAGE_SIZE) / PAGE_SIZE;
  bm_fill(bm, begin_index, end_index - begin_index, true);
}

void *alloc_pages(unsigned count)
{
  size_t n = bm_search(bm, count, false);
  if(n == BM_SEARCH_NONE)
    return ALLOC_FAILED;

  bm_fill(bm, n, count, true);
  return (void *)(PAGE_SIZE * n);
}

void free_pages(void *pages, unsigned count)
{
  size_t n = (uintptr_t)pages / PAGE_SIZE;
  bm_fill(bm, n, count, false);
}

