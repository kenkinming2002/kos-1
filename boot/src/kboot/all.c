#include "all.h"

#include <core/debug.h>

struct kboot_info *info;
void kboot_init(struct multiboot_boot_information *mbi)
{
  info = info_init(mbi);
  for(size_t i=0; i<info->mmap->count; ++i)
    debug_printf("info => mmap => entries[%lu] => { type = %lu, begin = 0x%lx, end = 0x%lx }\n",
        i,
        info->mmap->entries[i].type,
        info->mmap->entries[i].begin,
        info->mmap->entries[i].end);
}

