#include "mmap.h"

#include "alloc.h"

#include <core/assert.h>
#include <core/string.h>

static struct kboot_mmap_entry entries[256];
static size_t                  count;

static struct kboot_mmap_entry new_entries[256];
static size_t                  new_count;

static void mmap_append(struct kboot_mmap_entry entry)
{
  new_entries[new_count] = entry;
  ++new_count;
}

static void mmap_swap()
{
  memcpy(entries, new_entries, sizeof new_entries);
  count     = new_count;
  new_count = 0;
}

static void mmap_split_at(uint64_t addr)
{
  for(size_t i=0; i<count; ++i)
    if(entries[i].begin < addr && addr < entries[i].end)
    {
      struct kboot_mmap_entry new_entry;
      new_entry.type  = entries[i].type;
      new_entry.begin = entries[i].begin;
      new_entry.end   = addr;
      mmap_append(new_entry);
      new_entry.begin = addr;
      new_entry.end   = entries[i].end;
      mmap_append(new_entry);
    }
    else
      mmap_append(entries[i]);

  mmap_swap();
}

static void mmap_split()
{
  for(size_t i=0; i<count; ++i)
  {
    mmap_split_at(entries[i].begin);
    mmap_split_at(entries[i].end);
  }
}

static void mmap_sort()
{
  for(size_t i=1; i<count; ++i)
    for(size_t j=0; j<count-i; ++j)
      if(entries[j].begin > entries[j+1].begin)
      {
        struct kboot_mmap_entry tmp = entries[j];
        entries[j]   = entries[j+1];
        entries[j+1] = tmp;
      }
}

static void mmap_reduce()
{
  for(size_t i=0; i<count;)
  {
    struct kboot_mmap_entry new_entry = entries[i];

    size_t j;
    for(j=i+1; j<count; ++j)
    {
      if(new_entry.begin != entries[j].begin) break;
      if(new_entry.end   != entries[j].end)   break;
      if(entries[j].type != KBOOT_CONVENTIONAL_MEMORY)
        new_entry.type = entries[j].type;
    }
    i=j;

    mmap_append(new_entry);
  }
  mmap_swap();
}

static void mmap_merge()
{
  for(size_t i=0; i<count;)
  {
    struct kboot_mmap_entry new_entry = entries[i];

    size_t j;
    for(j=i+1; j<count; ++j)
    {
      if(new_entry.type != entries[j].type)  break;
      if(new_entry.end  != entries[j].begin) break;
      new_entry.end = entries[j].end;
    }
    i=j;

    mmap_append(new_entry);
  }
  mmap_swap();
}

static void mmap_sanitize()
{
  mmap_split();
  mmap_sort();
  mmap_reduce();
  mmap_merge();
}

static void mmap_populate(struct multiboot_boot_information *mbi)
{
  extern char system_structures_begin[];
  extern char system_structures_end[];
  extern char initrd_begin[];
  extern char initrd_end[];
  extern char kernel_begin[];
  extern char kernel_end[];
  extern char kboot_begin[];
  extern char kboot_end[];

  struct kboot_mmap_entry new_entry;

  MULTIBOOT_FOREACH_TAG(mbi, tag)
    if(tag->type == MULTIBOOT_TAG_TYPE_MMAP)
    {
      struct multiboot_tag_mmap *mmap_tag = (struct multiboot_tag_mmap *)tag;
      MULTIBOOT_FOREACH_MMAP_ENTRY(mmap_tag, kboot_mmap_entry)
      {
        switch(kboot_mmap_entry->type)
        {
        case MULTIBOOT_MEMORY_AVAILABLE:        new_entry.type = KBOOT_CONVENTIONAL_MEMORY; break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: new_entry.type = KBOOT_ACPI_RECLAIMABLE;    break;
        case MULTIBOOT_MEMORY_NVS:              new_entry.type = KBOOT_ACPI_NVS;            break;
        case MULTIBOOT_MEMORY_RESERVED:         new_entry.type = KBOOT_RESERVED;            break;
        case MULTIBOOT_MEMORY_BADRAM:           new_entry.type = KBOOT_BADRAM;              break;
        default:
          KASSERT_UNREACHABLE;
        }
        new_entry.begin = kboot_mmap_entry->addr;
        new_entry.end   = kboot_mmap_entry->addr + kboot_mmap_entry->len;
        mmap_append(new_entry);
      }
    }

  new_entry.type  = KBOOT_SYSTEM_STRUCTURES;
  new_entry.begin = (uintptr_t)system_structures_begin;
  new_entry.end   = (uintptr_t)system_structures_end;
  mmap_append(new_entry);

  new_entry.type  = KBOOT_INITRD;
  new_entry.begin = (uintptr_t)initrd_begin;
  new_entry.end   = (uintptr_t)initrd_end;
  mmap_append(new_entry);

  new_entry.type  = KBOOT_KERNEL;
  new_entry.begin = (uintptr_t)kernel_begin;
  new_entry.end   = (uintptr_t)kernel_end;
  mmap_append(new_entry);

  new_entry.type  = KBOOT_RECLAIMABLE;
  new_entry.begin = (uintptr_t)kboot_begin;
  new_entry.end   = (uintptr_t)kboot_end;
  mmap_append(new_entry);

  mmap_swap();
}

struct kboot_mmap *mmap_init(struct multiboot_boot_information *mbi)
{
  mmap_populate(mbi);
  mmap_sanitize();

  struct kboot_mmap *mmap = kboot_alloc(sizeof *mmap);
  mmap->entries = kboot_alloc(count * sizeof *mmap->entries);
  for(size_t i=0; i<count; ++i)
    mmap->entries[i] = entries[i];
  mmap->count = count;
  return mmap;
}

