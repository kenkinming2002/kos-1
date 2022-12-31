#ifndef BOOT_MULTIBOOT2_EXTRA_H
#define BOOT_MULTIBOOT2_EXTRA_H

#include <stdint.h>

#include "multiboot2.h"

/* Multiboot2 */
struct multiboot_boot_information
{
  uint32_t size;
  uint32_t reserved;
  struct multiboot_tag tags[];
};

#define MULTIBOOT_FOREACH_TAG(boot_info, tag) \
  for(struct multiboot_tag *tag = boot_info->tags; \
      tag->type != MULTIBOOT_TAG_TYPE_END; \
      tag = (struct multiboot_tag *)(((uintptr_t)tag + tag->size + 7) & (~7)))

#define MULTIBOOT_FOREACH_MMAP_ENTRY(mmap_tag, mmap_entry) \
  for(struct multiboot_mmap_entry *mmap_entry = mmap_tag->entries; \
      mmap_entry < (struct multiboot_mmap_entry *)((uintptr_t)mmap_tag   + mmap_tag->size); \
      mmap_entry = (struct multiboot_mmap_entry *)((uintptr_t)mmap_entry + mmap_tag->entry_size))

#endif // BOOT_MULTIBOOT2_EXTRA_H
