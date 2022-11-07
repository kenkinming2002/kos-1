#include "mm.h"

#include "core/assert.h"
#include "core/debug.h"

#include <stdbool.h>

static enum MemoryType as_memory_type(multiboot_uint32_t type)
{
  switch(type)
  {
  case MULTIBOOT_MEMORY_AVAILABLE:        return MEMORY_AVAILABLE;
  case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: return MEMORY_ACPI_RECLAIMABLE;
  case MULTIBOOT_MEMORY_NVS:              return MEMORY_ACPI_NVS;
  case MULTIBOOT_MEMORY_RESERVED:         return MEMORY_RESERVED;
  case MULTIBOOT_MEMORY_BADRAM:           return MEMORY_BAD;
  }
}

static struct mmap_entry mmap_entry_make_al(uintptr_t addr, size_t length, enum MemoryType type)  { return (struct mmap_entry){ .addr = addr,  .length = length,      .type = type }; }
static struct mmap_entry mmap_entry_make_be(uintptr_t begin, uintptr_t end, enum MemoryType type) { return (struct mmap_entry){ .addr = begin, .length = end - begin, .type = type }; }

static bool mmap_entry_contain(struct mmap_entry outer, struct mmap_entry inner)
{
  return outer.addr <= inner.addr && inner.addr + inner.length <= outer.addr + outer.length;
}

struct mmap_entry mmap_entries[MAX_MMAP_ENTRIES];
size_t            mmap_entry_count;

static void mmap_append_entry(struct mmap_entry new_entry)
{
  KASSERT(mmap_entry_count != MAX_MMAP_ENTRIES);
  mmap_entries[mmap_entry_count++] = new_entry;

  // Sort:
  for(size_t i=1; i<mmap_entry_count; ++i)
    for(size_t j=0; j<mmap_entry_count-i; ++j)
      if(mmap_entries[j].addr > mmap_entries[j+1].addr)
      {
        struct mmap_entry tmp = mmap_entries[j];
        mmap_entries[j]   = mmap_entries[j+1];
        mmap_entries[j+1] = tmp;
      }

  // Merge:
  size_t j=1;
  for(size_t i=1; i<mmap_entry_count; ++i)
    if(mmap_entries[j-1].type == mmap_entries[i].type && mmap_entries[j-1].addr + mmap_entries[j-1].length == mmap_entries[i].addr)
      mmap_entries[j-1].length += mmap_entries[i].length;
    else
      mmap_entries[j++] = mmap_entries[i];
  mmap_entry_count = j;
}

static void mmap_replace_entry(struct mmap_entry new_entry)
{
  debug_printf("replace entry addr=0x%lx, length=0x%lx\n", new_entry.addr, new_entry.length);
  for(size_t i=0; i<mmap_entry_count; ++i)
  {
    struct mmap_entry old_entry = mmap_entries[i];
    if(mmap_entry_contain(old_entry, new_entry))
    {
      struct mmap_entry new_entry1 = mmap_entry_make_be(old_entry.addr,                    new_entry.addr,                    old_entry.type);
      struct mmap_entry new_entry2 = mmap_entry_make_be(new_entry.addr + new_entry.length, old_entry.addr + old_entry.length, old_entry.type);
      mmap_entries[i] = new_entry;
      if(new_entry1.length) mmap_append_entry(new_entry1);
      if(new_entry2.length) mmap_append_entry(new_entry2);

      return;
    }
  }
  KASSERT(false && "Failed to replace region");
}

extern char boot_begin[];
extern char boot_end[];

void mm_init(struct multiboot_boot_information *boot_info)
{
  MULTIBOOT_FOREACH_TAG(boot_info, tag)
    if(tag->type == MULTIBOOT_TAG_TYPE_MMAP)
    {
      struct multiboot_tag_mmap *mmap_tag = (struct multiboot_tag_mmap *)tag;
      MULTIBOOT_FOREACH_MMAP_ENTRY(mmap_tag, entry)
        mmap_append_entry(mmap_entry_make_al(entry->addr, entry->len, as_memory_type(entry->type)));
    }

  MULTIBOOT_FOREACH_TAG(boot_info, tag)
    if(tag->type == MULTIBOOT_TAG_TYPE_MODULE)
    {
      struct multiboot_tag_module *module_tag = (struct multiboot_tag_module *)tag;
      mmap_replace_entry(mmap_entry_make_be(module_tag->mod_start, module_tag->mod_end, MEMORY_BOOTLOADER_RECLAIMABLE));
    }

  mmap_replace_entry(mmap_entry_make_be((uintptr_t)boot_begin, (uintptr_t)boot_end, MEMORY_BOOTLOADER_RECLAIMABLE));
}

void *mm_alloc(size_t count)
{
  for(size_t i=0; i<mmap_entry_count; ++i)
  {
    if(mmap_entries[i].type != MEMORY_AVAILABLE)
      continue;

    uintptr_t begin = (mmap_entries[i].addr + 0x1000 - 1) / 0x1000 * 0x1000;
    uintptr_t end   = begin + count * 0x1000;
    struct mmap_entry alloc_entry = mmap_entry_make_be(begin, end, MEMORY_BOOTLOADER_RECLAIMABLE);
    if(!mmap_entry_contain(mmap_entries[i], alloc_entry))
      continue;

    mmap_replace_entry(alloc_entry);
    return (void*)begin;
  }
}

