#include "pages.h"

#include <core/bitmap.h>

#include <stdalign.h>

#define PAGE_SIZE 0x1000

static struct bitmap bm;

#define ALIGN_UP(value, align)   ((value + align - 1) / align * align)
#define ALIGN_DOWN(value, align) (value / align * align)

static void mm_clear()
{
  bm_fill(bm, 0, bm.size, true);
}

static void mm_add(uintptr_t addr, size_t length)
{
  uintptr_t new_addr   = ALIGN_UP(addr, PAGE_SIZE);
  size_t    new_length = length - (new_addr - addr);
  if(addr + length < new_addr)
    return;

  bm_fill(bm, new_addr / PAGE_SIZE, ALIGN_DOWN(new_length, PAGE_SIZE) / PAGE_SIZE, false);
}

static void mm_del(uintptr_t addr, size_t length)
{
  uintptr_t new_addr   = ALIGN_DOWN(addr, PAGE_SIZE);
  size_t    new_length = length + (addr - new_addr);
  bm_fill(bm, new_addr / PAGE_SIZE, ALIGN_UP(new_length, PAGE_SIZE) / PAGE_SIZE, true);
}

static uintptr_t max_addr = 0;
static void iterate_get_max_addr(struct boot_mmap_entry *entry)
{
  if(max_addr < entry->addr)
    max_addr = entry->addr;
}

static void iterate_init(struct boot_mmap_entry *entry)
{
  if(entry->type == BOOT_MEMORY_CONVENTIONAL && entry->owner == BOOT_MEMORY_UNOWNED)
    mm_add(entry->addr, entry->length);
}

extern char kernel_begin[];
extern char kernel_end[];

void mm_init_pages(struct boot_service *boot_service)
{
  boot_service->mm_iterate(&iterate_get_max_addr);

  size_t page_count = (max_addr + PAGE_SIZE - 1) / PAGE_SIZE;
  bm.size = page_count;
  bm.bits = boot_service->mm_alloc(page_count / CHAR_BIT, alignof(unsigned));

  mm_clear();
  boot_service->mm_iterate(&iterate_init);
  mm_del((uintptr_t)kernel_begin, kernel_end - kernel_begin);
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

