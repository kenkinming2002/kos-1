#include "mmap.h"

#include "core/assert.h"

struct mmap_entry mmap_entries[MAX_MMAP_ENTRIES];
size_t            mmap_entry_count;

static void mmap_sanitize()
{
  // TODO:
}

static enum MemoryType as_memory_type(multiboot_uint32_t type)
{
  switch(type)
  {
  case MULTIBOOT_MEMORY_AVAILABLE:        return MEMORY_AVAILABLE;
  case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: return MEMORY_ACPI_RECLAIMABLE;
  case MULTIBOOT_MEMORY_NVS:              return MEMORY_ACPI_NVS;
  case MULTIBOOT_MEMORY_RESERVED:         return MEMORY_RESERVED_SYSTEM;
  case MULTIBOOT_MEMORY_BADRAM:           return MEMORY_BAD;
  }
}

void mmap_init(struct multiboot_boot_information *boot_info)
{
  MULTIBOOT_FOREACH_TAG(boot_info, tag)
  {
    if(tag->type == MULTIBOOT_TAG_TYPE_MMAP)
    {
      struct multiboot_tag_mmap *mmap_tag = (struct multiboot_tag_mmap *)tag;
      MULTIBOOT_FOREACH_MMAP_ENTRY(mmap_tag, entry)
      {
        KASSERT(mmap_entry_count < MAX_MMAP_ENTRIES);
        mmap_entries[mmap_entry_count++] = (struct mmap_entry){
          .addr = entry->addr,
          .length = entry->len,
          .type = as_memory_type(entry->type),
        };
      }
    }
  }
}
