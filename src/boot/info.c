#include "info.h"

#include "alloc.h"

#include "core/assert.h"
#include "core/print.h"
#include "core/string.h"

#include "multiboot2.h"

#include <stdalign.h>

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

static struct multiboot_tag *multiboot_find_tag(struct multiboot_boot_information *boot_info, multiboot_uint32_t type)
{
  MULTIBOOT_FOREACH_TAG(boot_info, tag)
  {
    if(tag->type == type)
      return tag;
  }
  debug_printf("Failed to find tag\n");
  return NULL;
}

struct multiboot_boot_information *multiboot_boot_information_get(uint32_t addr)
{
  struct multiboot_boot_information *tmp = (struct multiboot_boot_information *)addr;
  struct multiboot_boot_information *boot_info = alloc_heap(tmp->size, 8);
  memmove(boot_info, tmp, tmp->size);
  return boot_info;
}

struct boot_info_mmap *prepare_boot_info_mmap(struct multiboot_boot_information *mb_boot_info)
{
  struct multiboot_tag_mmap *mb_mmap_tag = (struct multiboot_tag_mmap *)multiboot_find_tag(mb_boot_info, MULTIBOOT_TAG_TYPE_MMAP);
  KASSERT(mb_mmap_tag);

  struct boot_info_mmap *mmap = NULL;

  size_t count = 0;
  MULTIBOOT_FOREACH_MMAP_ENTRY(mb_mmap_tag, mb_mmap_entry) { ++count; }
  mmap = alloc_memory(sizeof *mmap + sizeof *mmap->entries * count, alignof *mmap);
  mmap->count = count;

  size_t i = 0;
  MULTIBOOT_FOREACH_MMAP_ENTRY(mb_mmap_tag, mb_mmap_entry) {
    mmap->entries[i].addr = mb_mmap_entry->addr;
    mmap->entries[i].size = mb_mmap_entry->len;
    ++i;
  }

  return mmap;
}

struct boot_info_alloc *prepare_boot_info_alloc()
{
  struct boot_info_alloc *alloc = alloc_memory(sizeof *alloc, alignof *alloc);
  memset(alloc, 0, sizeof *alloc);
  return alloc;
}

struct boot_info *prepare_boot_info(struct multiboot_boot_information *mb_boot_info)
{
  struct boot_info *boot_info = alloc_memory(sizeof *boot_info, alignof *boot_info);
  boot_info->mmap  = prepare_boot_info_mmap(mb_boot_info);
  boot_info->alloc = prepare_boot_info_alloc();
  return boot_info;
}

