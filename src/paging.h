#ifndef PAGING_H
#define PAGING_H

#include "core/assert.h"

#include <stdint.h>

#define PAGE_SHIFT      12
#define HUGE_PAGE_SHIFT 22

#define PAGE_SIZE      1 << PAGE_SHIFT
#define HUGE_PAGE_SIZE 1 << HUGE_PAGE_SHIFT

enum PageCacheMode
{
  PAGE_CACHED,
  PAGE_UNCACHED,
};

enum PageWriteMode
{
  PAGE_WRITE_THROUGH,
  PAGE_WRITE_BACK,
};

enum PageAccess
{
  PAGE_ACCESS_ANY,
  PAGE_ACCESS_SUPERVISOR,
};

enum PagePermission
{
  PAGE_READ_ONLY,
  PAGE_READ_WRITE
};

enum PageTLBMode
{
  PAGE_TLB_GLOBAL,
  PAGE_TLB_LOCAL
};

struct pte { uint32_t data; };
struct pde { uint32_t data; };

static inline uint32_t page_entry_make(
    uint32_t address, uint32_t address_mask,
    enum PagePermission permission,
    enum PageAccess     access,
    enum PageWriteMode  write_mode,
    enum PageCacheMode  cache_mode)
{
  KASSERT((address & address_mask) == address);

  uint32_t data = 0;
  data |= address;
  data |= (cache_mode == PAGE_UNCACHED)      << 4;
  data |= (write_mode == PAGE_WRITE_THROUGH) << 3;
  data |= (access     == PAGE_ACCESS_ANY)    << 2;
  data |= (permission == PAGE_READ_WRITE)    << 1;
  data |= 0x1; // present
  return data;
}

static inline struct pte page_table_entry_make(
    uint32_t address,
    enum PagePermission permission,
    enum PageAccess     access,
    enum PageWriteMode  write_mode,
    enum PageCacheMode  cache_mode)
{
  struct pte pte = {0};
  pte.data = page_entry_make(address, 0xFFFFF000, permission, access, write_mode, cache_mode);
  return pte;
}

static inline struct pde page_directory_entry_make(
    uint32_t address,
    enum PageCacheMode cache_mode,
    enum PageWriteMode write_mode,
    enum PageAccess access,
    enum PagePermission permission)
{
  struct pde pde = {0};
  pde.data = page_entry_make(address, 0xFFFFF000, permission, access, write_mode, cache_mode);
  return pde;
}

static inline struct pde page_directory_entry_make_huge_page(
    uint32_t address,
    enum PageCacheMode cache_mode,
    enum PageWriteMode write_mode,
    enum PageAccess access,
    enum PagePermission permission)
{
  struct pde pde = {0};
  pde.data = page_entry_make(address, 0xFFC00000, permission, access, write_mode, cache_mode);
  pde.data |= 0x00000080;
  return pde;
}


#endif // PAGING_H
