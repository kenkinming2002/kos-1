#include "mmap.h"

#include "core/assert.h"

struct mmap mmap;

/* mem region */
static void mmap_insert(uintptr_t addr, size_t length, enum MemoryType type)
{
  KASSERT(mmap.count < MAX_MMAP_ENTRIES);
  mmap.entries[mmap.count++] = (struct mmap_entry){.addr = addr, .length = length, .type = type};
}

static void mmap_sanitize()
{
  // TODO:
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
        switch(entry->type)
        {
        case MULTIBOOT_MEMORY_AVAILABLE:        mmap_insert(entry->addr, entry->len, MEMORY_AVAILABLE);        break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: mmap_insert(entry->addr, entry->len, MEMORY_ACPI_RECLAIMABLE); break;
        case MULTIBOOT_MEMORY_NVS:              mmap_insert(entry->addr, entry->len, MEMORY_ACPI_NVS);         break;
        case MULTIBOOT_MEMORY_RESERVED:         mmap_insert(entry->addr, entry->len, MEMORY_RESERVED);         break;
        case MULTIBOOT_MEMORY_BADRAM:           mmap_insert(entry->addr, entry->len, MEMORY_BAD);              break;
        }
      }
    }
  }
}
