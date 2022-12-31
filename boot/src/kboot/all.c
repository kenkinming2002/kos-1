#include "all.h"

#include <core/debug.h>

void kboot_init(struct multiboot_boot_information *mbi, kboot_info_t *kboot_info)
{
  *kboot_info = info_init(mbi);
  for(size_t i=0; i<(*kboot_info)->mmap->count; ++i)
    debug_printf("info => mmap => entries[%lu] => { type = %lu, begin = 0x%lx, end = 0x%lx }\n",
        i,
        (*kboot_info)->mmap->entries[i].type,
        (*kboot_info)->mmap->entries[i].begin,
        (*kboot_info)->mmap->entries[i].end);
}

