#include "mm.h"

#include "core/assert.h"
#include "core/debug.h"

#include <stdbool.h>

#define MMAP_ENTRY_AL(_type, _owner, _addr, _length) (struct boot_mmap_entry){ .type = (_type), .owner = (_owner), .addr = (_addr),  .length = (_length) }
#define MMAP_ENTRY_BE(_type, _owner, _begin, _end)   (struct boot_mmap_entry){ .type = (_type), .owner = (_owner), .addr = (_begin), .length = (_end) - (_begin) }

// MMAP
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
    else if(entries[i].length != 0)
      entries[j++] = entries[i];
  count = j;
}

// Services
void boot_mm_iterate(void(*iterate)(struct boot_mmap_entry *mmap_entry))
{
  for(size_t i=0; i<count; ++i)
    iterate(&entries[i]);
}

uintptr_t boot_mm_lookup(size_t length, size_t alignment, enum boot_memory_type type, enum boot_memory_owner owner)
{
  for(size_t i=0; i<count; ++i)
  {
    uintptr_t addr = (entries[i].addr + alignment - 1) / alignment * alignment;
    if(entries[i].type  == type  &&
       entries[i].owner == owner &&
       entries[i].addr <= addr && addr + length <= entries[i].addr + entries[i].length)
      return addr;
  }
  KASSERT_UNREACHABLE;
}

void boot_mm_transfer(uintptr_t addr, size_t length, enum boot_memory_type type, enum boot_memory_owner from, enum boot_memory_owner to)
{
  for(size_t i=0; i<count; ++i)
  {
    if(entries[i].type == type &&
       entries[i].owner == from &&
       entries[i].addr <= addr && addr + length <= entries[i].addr + entries[i].length)
    {
      struct boot_mmap_entry entry1 = MMAP_ENTRY_BE(type, from, entries[i].addr, addr);
      struct boot_mmap_entry entry2 = MMAP_ENTRY_BE(type, to,   addr, addr + length);
      struct boot_mmap_entry entry3 = MMAP_ENTRY_BE(type, from, addr + length, entries[i].addr + entries[i].length);
      entries[i].length = 0;
      if(entry1.length) mmap_append_entry(entry1);
      if(entry2.length) mmap_append_entry(entry2);
      if(entry3.length) mmap_append_entry(entry3);
      mmap_sanitize();
      return;
    }
  }
  KASSERT_UNREACHABLE;
}

// Init
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
      boot_mm_transfer(module_tag->mod_start, module_tag->mod_end - module_tag->mod_start, BOOT_MEMORY_CONVENTIONAL,
          BOOT_MEMORY_UNOWNED,
          BOOT_MEMORY_BOOTLOADER);
    }

  boot_mm_transfer((uintptr_t)boot_begin, boot_end - boot_begin, BOOT_MEMORY_CONVENTIONAL,
      BOOT_MEMORY_UNOWNED,
      BOOT_MEMORY_BOOTLOADER);

  mmap_debug();
  // 3: Testing
  for(size_t i=0; i<32; ++i)
    debug_printf("alloc(0x%lx) = 0x%lx\n", i * 400, (uintptr_t)boot_mm_alloc(i * 400, 16));
  mmap_debug();
}

void* boot_mm_alloc(size_t size, size_t alignment)
{
  uintptr_t addr = boot_mm_lookup(size, alignment, BOOT_MEMORY_CONVENTIONAL, BOOT_MEMORY_UNOWNED);
  boot_mm_transfer(addr, size, BOOT_MEMORY_CONVENTIONAL, BOOT_MEMORY_UNOWNED, BOOT_MEMORY_KERNEL);

  debug_printf("alloc(size=0x%lx,alignment=0x%lx) = 0x%lx\n", size, alignment, addr);
  return (void*)addr;
}

void boot_mm_free(void *ptr, size_t size)
{
  uintptr_t addr = (uintptr_t)ptr;
  boot_mm_transfer(addr, size, BOOT_MEMORY_CONVENTIONAL, BOOT_MEMORY_KERNEL, BOOT_MEMORY_UNOWNED);
  debug_printf("free(ptr=0x%lx,size=0x%lx)\n", (uintptr_t)ptr, size);
}


