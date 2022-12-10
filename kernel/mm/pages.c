#include "pages.h"

#include <core/bitmap.h>

#include <stdalign.h>

#define PAGE_SIZE 0x1000

static struct bitmap bm;

#define ALIGN_UP(value, align)   ((value + align - 1) / align * align)
#define ALIGN_DOWN(value, align) (value / align * align)

static uintptr_t max_addr = 0;
static void iterate_get_max_addr(struct boot_mmap_entry *entry)
{
  if(max_addr < entry->addr)
    max_addr = entry->addr;
}

static void iterate_init(struct boot_mmap_entry *entry)
{
  if(entry->type == BOOT_MEMORY_CONVENTIONAL && entry->owner == BOOT_MEMORY_UNOWNED)
  {
    size_t begin_index = ALIGN_UP  (entry->addr,                 PAGE_SIZE) / PAGE_SIZE;
    size_t end_index   = ALIGN_DOWN(entry->addr + entry->length, PAGE_SIZE) / PAGE_SIZE;
    if(begin_index > end_index)
      return;

    bm_fill(bm, begin_index, end_index - begin_index, false);
  }
}

void mm_init_pages(struct boot_service *boot_service)
{
  boot_service->mm_iterate(&iterate_get_max_addr);

  size_t page_count = (max_addr + PAGE_SIZE - 1) / PAGE_SIZE;
  bm.size = page_count;
  bm.bits = boot_service->mm_alloc(page_count / CHAR_BIT, alignof(unsigned));
  bm_fill(bm, 0, bm.size, true);

  boot_service->mm_iterate(&iterate_init);
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

