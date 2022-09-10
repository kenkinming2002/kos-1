#ifndef TYPES_PAGING_H
#define TYPES_PAGING_H

#include <stdint.h>

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

// You could set the global bit for both page table entry and page directory entry
// It is just that the global bit only matter for page table entry and page directory entry that refer to a huge page
//
struct pte page_table_entry_make(
    enum PagePermission permission,
    enum PageAccess     access,
    enum PageWriteMode  write_mode,
    enum PageCacheMode  cache_mode,
    uint32_t address);

struct pde page_directory_entry_make(
    uint32_t address,
    enum PageCacheMode cache_mode,
    enum PageWriteMode write_mode,
    enum PageAccess access,
    enum PagePermission permission);

#endif // TYPES_PAGING_H
