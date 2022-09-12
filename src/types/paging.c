#include "paging.h"

#include "core/assert.h"

static uint32_t page_entry_make(
    enum PagePermission permission,
    enum PageAccess     access,
    enum PageWriteMode  write_mode,
    enum PageCacheMode  cache_mode,
    uint32_t address, uint32_t address_mask)
{
  KASSERT((address & address_mask) != address);

  uint32_t data = 0;
  data |= address;
  data |= (cache_mode == PAGE_UNCACHED)      << 4;
  data |= (write_mode == PAGE_WRITE_THROUGH) << 3;
  data |= (access     == PAGE_ACCESS_ANY)    << 2;
  data |= (permission == PAGE_READ_WRITE)    << 1;
  data |= 0x1; // present
  return data;
}


struct pte page_table_entry_make(
    enum PagePermission permission,
    enum PageAccess     access,
    enum PageWriteMode  write_mode,
    enum PageCacheMode  cache_mode,
    uint32_t address)
{
  struct pte pte = {0};
  pte.data = page_entry_make(permission, access, write_mode, cache_mode, address, 0xFFFFF000);
  return pte;
}

struct pde page_directory_entry_make(
    uint32_t address,
    enum PageCacheMode cache_mode,
    enum PageWriteMode write_mode,
    enum PageAccess access,
    enum PagePermission permission)
{
  struct pde pde = {0};
  pde.data = page_entry_make(permission, access, write_mode, cache_mode, address, 0xFFFFF000);
  return pde;
}
