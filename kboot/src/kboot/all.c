#include "all.h"

#include <rt/core/log.h>

void kboot_init(struct multiboot_boot_information *mbi, kboot_info_t *kboot_info)
{
  *kboot_info = info_init(mbi);
  for(size_t i=0; i<(*kboot_info)->mmap->count; ++i)
    logf(LOG_TRACE "kboot_info - mmap - entries[{:lu}] - type = {:lu}, begin = 0x{X:lu}, end = 0x{X:lu}\n",
        i,
        (*kboot_info)->mmap->entries[i].type,
        (*kboot_info)->mmap->entries[i].begin,
        (*kboot_info)->mmap->entries[i].end);
}

