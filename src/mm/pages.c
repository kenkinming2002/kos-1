#include "pages.h"

#include "bitmap.h"
#include "boot_params.h"
#include "linear.h"

#include "core/assert.h"
#include "core/print.h"
#include "core/string.h"

extern char boot_begin[];
extern char boot_end[];

extern char kernel_begin[];
extern char kernel_end[];

#define BOOT_BEGIN ((void *)boot_begin)
#define BOOT_END   ((void *)boot_end)
#define BOOT_LENGTH ((uintptr_t)BOOT_END - (uintptr_t)BOOT_BEGIN)

#define KERNEL_BEGIN ((void *)kernel_begin)
#define KERNEL_END   ((void *)kernel_end)
#define KERNEL_LENGTH ((uintptr_t)KERNEL_END - (uintptr_t)KERNEL_BEGIN)

#define ALIGN_UP(value, align)   ((value + align - 1) / align * align)
#define ALIGN_DOWN(value, align) (value / align * align)

static struct bitmap bm;

static void mm_add_region(uintptr_t addr, size_t length)
{
  uintptr_t new_addr   = ALIGN_UP(addr, PAGE_SIZE);
  size_t    new_length = length - (new_addr - addr);
  bm_fill(bm, new_addr / PAGE_SIZE, ALIGN_DOWN(new_length, PAGE_SIZE) / PAGE_SIZE, false);
}

static void mm_del_region(uintptr_t addr, size_t length)
{
  uintptr_t new_addr   = ALIGN_DOWN(addr, PAGE_SIZE);
  size_t    new_length = length + (addr - new_addr);
  bm_fill(bm, new_addr / PAGE_SIZE, ALIGN_UP(new_length, PAGE_SIZE) / PAGE_SIZE, true);
}

void mm_init_pages_allocator()
{
  // 1: Find maximum page frame number
  uintptr_t max_addr = 0;
  for(size_t i=0; i<boot_params.mmap_entry_count; ++i)
    if(max_addr < boot_params.mmap_entries[i].addr)
      max_addr = boot_params.mmap_entries[i].addr;

  size_t page_count = max_addr / PAGE_SIZE;

  bm.size = page_count;
  bm.bits = (unsigned *)kernel_end;
  bm_fill(bm, 0, bm.size, true);

  for(size_t i=0; i<boot_params.mmap_entry_count; ++i)
    if(boot_params.mmap_entries[i].type == MEMORY_AVAILABLE)
      mm_add_region(boot_params.mmap_entries[i].addr, boot_params.mmap_entries[i].length);

  mm_del_region(virt_to_phys(BOOT_BEGIN),   BOOT_LENGTH);
  mm_del_region(virt_to_phys(KERNEL_BEGIN), KERNEL_LENGTH);
  mm_del_region(virt_to_phys(KERNEL_END),   page_count * sizeof(unsigned) / UINT_BIT);
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

