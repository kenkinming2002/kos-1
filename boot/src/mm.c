#include "mm.h"

#include "core/assert.h"
#include "core/debug.h"

#include <stdbool.h>

// Helpers
static struct boot_mmap_entry mmap_entry_make_al(uintptr_t addr, size_t length, enum boot_memory_type type)
{
  return (struct boot_mmap_entry){
    .addr = addr,
    .length = length,
    .type = type
  };
}

static struct boot_mmap_entry mmap_entry_make_be(uintptr_t begin, uintptr_t end, enum boot_memory_type type)
{
  return (struct boot_mmap_entry){
    .addr = begin,
    .length = end - begin,
    .type = type
  };
}

static bool mmap_entry_contain(struct boot_mmap_entry outer, struct boot_mmap_entry inner)
{
  return outer.addr <= inner.addr && inner.addr + inner.length <= outer.addr + outer.length;
}

// Basics
#define MAX_MMAP_ENTRIES 128
static struct boot_mmap_entry entries[MAX_MMAP_ENTRIES];
static size_t                 count;

static void mmap_append_entry(struct boot_mmap_entry new_entry)
{
  KASSERT(count != MAX_MMAP_ENTRIES);
  entries[count++] = new_entry;
}

static void mmap_replace_entry(struct boot_mmap_entry new_entry)
{
  for(size_t i=0; i<count; ++i)
  {
    struct boot_mmap_entry old_entry = entries[i];
    if(mmap_entry_contain(old_entry, new_entry))
    {
      struct boot_mmap_entry new_entry1 = mmap_entry_make_be(old_entry.addr,                    new_entry.addr,                    old_entry.type);
      struct boot_mmap_entry new_entry2 = mmap_entry_make_be(new_entry.addr + new_entry.length, old_entry.addr + old_entry.length, old_entry.type);
      entries[i] = new_entry;
      if(new_entry1.length) mmap_append_entry(new_entry1);
      if(new_entry2.length) mmap_append_entry(new_entry2);

      return;
    }
  }
  KASSERT(false && "Failed to replace region");
}

static void mmap_sanitize()
{
  // sort
  for(size_t i=1; i<count; ++i)
    for(size_t j=0; j<count-i; ++j)
      if(entries[j].addr > entries[j+1].addr)
      {
        struct boot_mmap_entry tmp = entries[j];
        entries[j]   = entries[j+1];
        entries[j+1] = tmp;
      }

  // merge
  size_t j=1;
  for(size_t i=1; i<count; ++i)
    if(entries[j-1].type == entries[i].type && entries[j-1].addr + entries[j-1].length == entries[i].addr)
      entries[j-1].length += entries[i].length;
    else
      entries[j++] = entries[i];
  count = j;
}

// API
static enum boot_memory_type as_memory_type(multiboot_uint32_t type)
{
  switch(type)
  {
  case MULTIBOOT_MEMORY_AVAILABLE:        return BOOT_MEMORY_AVAILABLE;
  case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: return BOOT_MEMORY_ACPI_RECLAIMABLE;
  case MULTIBOOT_MEMORY_NVS:              return BOOT_MEMORY_ACPI_NVS;
  case MULTIBOOT_MEMORY_RESERVED:         return BOOT_MEMORY_RESERVED;
  case MULTIBOOT_MEMORY_BADRAM:           return BOOT_MEMORY_BAD;
  }
  KASSERT_UNREACHABLE;
}

static const char *as_string(enum boot_memory_type type)
{
  switch(type)
  {
    case BOOT_MEMORY_AVAILABLE:              return "available";
    case BOOT_MEMORY_BOOTLOADER_RECLAIMABLE: return "bootloader reclaimable";
    case BOOT_MEMORY_BOOTLOADER_FILESYSTEM:  return "bootloader filesystem";
    case BOOT_MEMORY_ACPI_RECLAIMABLE:       return "acpi reclaimable";
    case BOOT_MEMORY_ACPI_NVS:               return "acpi nvs";
    case BOOT_MEMORY_RESERVED:               return "reserved";
    case BOOT_MEMORY_BAD:                    return "bad";
  }
  KASSERT_UNREACHABLE;
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
  mmap_sanitize();

  MULTIBOOT_FOREACH_TAG(boot_info, tag)
    if(tag->type == MULTIBOOT_TAG_TYPE_MODULE)
    {
      struct multiboot_tag_module *module_tag = (struct multiboot_tag_module *)tag;
      mmap_replace_entry(mmap_entry_make_be(module_tag->mod_start, module_tag->mod_end, BOOT_MEMORY_BOOTLOADER_FILESYSTEM));
    }
  mmap_replace_entry(mmap_entry_make_be((uintptr_t)boot_begin, (uintptr_t)boot_end, BOOT_MEMORY_BOOTLOADER_RECLAIMABLE));
  mmap_sanitize();

  for(size_t i=0; i<32; ++i)
  {
    debug_printf("mmap\n");
    for(size_t i=0; i<count; ++i)
      debug_printf(" => addr=0x%lx, length=0x%lx, type=%s\n",
          entries[i].addr,
          entries[i].length,
          as_string(entries[i].type));

    debug_printf("alloc_pages(%lu) = 0x%lx\n", i, (uintptr_t)boot_mm_alloc_pages(i));
  }
}

void boot_mm_iterate(void(*iterate)(struct boot_mmap_entry *mmap_entry))
{
  for(size_t i=0; i<count; ++i)
    iterate(&entries[i]);
}

void *boot_mm_alloc_pages(size_t alloc_count)
{
  for(size_t i=0; i<count; ++i)
  {
    if(entries[i].type != BOOT_MEMORY_AVAILABLE)
      continue;

    uintptr_t begin = (entries[i].addr + 0x1000 - 1) / 0x1000 * 0x1000;
    struct boot_mmap_entry alloc_entry = mmap_entry_make_al(begin, alloc_count * 0x1000, BOOT_MEMORY_BOOTLOADER_RECLAIMABLE);
    if(!mmap_entry_contain(entries[i], alloc_entry))
      continue;

    mmap_replace_entry(alloc_entry);
    mmap_sanitize();
    return (void*)begin;
  }
  KASSERT(false && "Unreachable");
}

void boot_mm_free_pages(void *ptr, size_t alloc_count)
{
  struct boot_mmap_entry alloc_entry = mmap_entry_make_al((uintptr_t)ptr, alloc_count * 0x1000, BOOT_MEMORY_AVAILABLE);
  mmap_replace_entry(alloc_entry);
  mmap_sanitize();
}


