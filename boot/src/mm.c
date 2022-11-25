#include "mm.h"

#include "core/assert.h"
#include "core/debug.h"

#include <stdbool.h>

#define MMAP_ENTRY_AL(_type, _owner, _addr, _length) (struct boot_mmap_entry){ .type = (_type), .owner = (_owner), .addr = (_addr),  .length = (_length) }
#define MMAP_ENTRY_BE(_type, _owner, _begin, _end)   (struct boot_mmap_entry){ .type = (_type), .owner = (_owner), .addr = (_begin), .length = (_end) - (_begin) }

static bool mmap_entry_contain(struct boot_mmap_entry outer, struct boot_mmap_entry inner)
{
  return outer.addr <= inner.addr && inner.addr + inner.length <= outer.addr + outer.length;
}

// Basics
#define MAX_MMAP_ENTRIES 128
static struct boot_mmap_entry entries[MAX_MMAP_ENTRIES];
static size_t                 count;

static void mmap_debug()
{
  debug_printf("mmap\n");
  for(size_t i=0; i<count; ++i)
  {
    const char *type_str  = NULL;
    switch(entries[i].type)
    {
      case BOOT_MEMORY_CONVENTIONAL: type_str = "conventional"; break;
      case BOOT_MEMORY_RESERVED:     type_str = "reserved    "; break;
      case BOOT_MEMORY_NVS:          type_str = "nvs         "; break;
      case BOOT_MEMORY_BAD:          type_str = "bad         "; break;
      default: KASSERT_UNREACHABLE;
    }
    const char *owner_str = NULL;
    switch(entries[i].owner)
    {
      case BOOT_MEMORY_UNOWNED:    owner_str = "unowned   "; break;
      case BOOT_MEMORY_ACPI:       owner_str = "acpi      "; break;
      case BOOT_MEMORY_BOOTLOADER: owner_str = "bootloader"; break;
      case BOOT_MEMORY_KERNEL:     owner_str = "kernel    "; break;
      default: KASSERT_UNREACHABLE;
    }
    debug_printf(" => type=%s, owner=%s, addr=0x%lx, length=0x%lx\n", type_str, owner_str, entries[i].addr, entries[i].length);
  }
}

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
      struct boot_mmap_entry new_entry1 = MMAP_ENTRY_BE(old_entry.type, old_entry.owner, old_entry.addr,                    new_entry.addr);
      struct boot_mmap_entry new_entry2 = MMAP_ENTRY_BE(old_entry.type, old_entry.owner, new_entry.addr + new_entry.length, old_entry.addr + old_entry.length);
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
    if(entries[j-1].type == entries[i].type && entries[j-1].owner == entries[i].owner && entries[j-1].addr + entries[j-1].length == entries[i].addr)
      entries[j-1].length += entries[i].length;
    else
      entries[j++] = entries[i];
  count = j;
}

void mm_init(struct multiboot_boot_information *boot_info)
{
  // 1: Mark memory as in multiboot2 memory map
  MULTIBOOT_FOREACH_TAG(boot_info, tag)
    if(tag->type == MULTIBOOT_TAG_TYPE_MMAP)
    {
      struct multiboot_tag_mmap *mmap_tag = (struct multiboot_tag_mmap *)tag;
      MULTIBOOT_FOREACH_MMAP_ENTRY(mmap_tag, entry)
      {
        enum boot_memory_type  type;
        enum boot_memory_owner owner;
        switch(entry->type)
        {
        case MULTIBOOT_MEMORY_AVAILABLE:
          type  = BOOT_MEMORY_CONVENTIONAL;
          owner = BOOT_MEMORY_UNOWNED;
          break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
          type = BOOT_MEMORY_CONVENTIONAL;
          owner = BOOT_MEMORY_ACPI;
          break;
        case MULTIBOOT_MEMORY_NVS:
          type = BOOT_MEMORY_NVS;
          owner = BOOT_MEMORY_ACPI;
          break;
        case MULTIBOOT_MEMORY_RESERVED:
          type = BOOT_MEMORY_RESERVED;
          owner = BOOT_MEMORY_UNOWNED;
          break;
        case MULTIBOOT_MEMORY_BADRAM:
          type  = BOOT_MEMORY_BAD;
          owner = BOOT_MEMORY_UNOWNED;
          break;
        default:
          KASSERT_UNREACHABLE;
        }
        mmap_append_entry(MMAP_ENTRY_AL(type, owner, entry->addr, entry->len));
      }
    }
  mmap_sanitize();

  // 2: Mark memory used by bootloader
  extern char boot_begin[];
  extern char boot_end[];

  MULTIBOOT_FOREACH_TAG(boot_info, tag)
    if(tag->type == MULTIBOOT_TAG_TYPE_MODULE)
    {
      struct multiboot_tag_module *module_tag = (struct multiboot_tag_module *)tag;
      mmap_replace_entry(MMAP_ENTRY_BE(BOOT_MEMORY_CONVENTIONAL, BOOT_MEMORY_BOOTLOADER, module_tag->mod_start, module_tag->mod_end));
    }
  mmap_replace_entry(MMAP_ENTRY_BE(BOOT_MEMORY_CONVENTIONAL, BOOT_MEMORY_BOOTLOADER, (uintptr_t)boot_begin, (uintptr_t)boot_end));
  mmap_sanitize();

  mmap_debug();

  // 3: Testing
  for(size_t i=0; i<32; ++i)
    debug_printf("alloc_pages(%lu) = 0x%lx\n", i, (uintptr_t)boot_mm_alloc_pages(i));

  mmap_debug();
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
    if(entries[i].type != BOOT_MEMORY_CONVENTIONAL || entries[i].owner != BOOT_MEMORY_UNOWNED)
      continue;

    uintptr_t begin  = (entries[i].addr + 0x1000 - 1) / 0x1000 * 0x1000;
    uintptr_t length = alloc_count * 0x1000;
    struct boot_mmap_entry alloc_entry = MMAP_ENTRY_AL(BOOT_MEMORY_CONVENTIONAL, BOOT_MEMORY_KERNEL, begin, length);

    if(!mmap_entry_contain(entries[i], alloc_entry)) continue;

    mmap_replace_entry(alloc_entry);
    mmap_sanitize();
    return (void*)begin;
  }
  KASSERT(false && "Unreachable");
}

void boot_mm_free_pages(void *ptr, size_t alloc_count)
{
  uintptr_t begin = (uintptr_t)ptr;
  uintptr_t length = alloc_count * 0x1000;
  struct boot_mmap_entry alloc_entry = MMAP_ENTRY_AL(BOOT_MEMORY_CONVENTIONAL, BOOT_MEMORY_KERNEL, begin, length);
  mmap_replace_entry(alloc_entry);
  mmap_sanitize();
}


