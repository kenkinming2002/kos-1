#include "boot_params.h"

#include "core/assert.h"
#include "core/string.h"

#include "mm/linear.h"
#include "multiboot2.h"

/* mem region */
static void mem_region_insert(struct mem_region *region, uintptr_t addr, size_t length)
{
  KASSERT(region->count < MAX_MEM_AREA_COUNT);
  region->areas[region->count++] = (struct mem_area){.addr = addr, .length = length};
}

static void mem_region_remove(struct mem_region *region, uintptr_t addr, size_t length)
{
  // TODO:
}

static void mem_region_sanitize(struct mem_region *arr)
{
  // TODO:
}

struct boot_params boot_params;

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

static void boot_params_init_multiboot2_cmdline(struct multiboot_tag_string *tag)
{
  kstrcpy(boot_params.cmdline, tag->string, sizeof boot_params.cmdline);
}

static void boot_params_init_multiboot2_mmap(struct multiboot_tag_mmap *tag)
{
  MULTIBOOT_FOREACH_MMAP_ENTRY(tag, entry)
  {
    switch(entry->type)
    {
    case MULTIBOOT_MEMORY_AVAILABLE:        mem_region_insert(&boot_params.mmap.usable,           entry->addr, entry->len); break;
    case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: mem_region_insert(&boot_params.mmap.acpi_reclaimable, entry->addr, entry->len); break;
    case MULTIBOOT_MEMORY_NVS:              mem_region_insert(&boot_params.mmap.acpi_nvs,         entry->addr, entry->len); break;
    case MULTIBOOT_MEMORY_RESERVED:         mem_region_insert(&boot_params.mmap.reserved,         entry->addr, entry->len); break;
    case MULTIBOOT_MEMORY_BADRAM:           mem_region_insert(&boot_params.mmap.bad,              entry->addr, entry->len); break;
    }
  }
}

static void boot_params_init_multiboot2(struct multiboot_boot_information *boot_info)
{
  MULTIBOOT_FOREACH_TAG(boot_info, tag)
    switch(tag->type)
    {
    case MULTIBOOT_TAG_TYPE_CMDLINE:
      boot_params_init_multiboot2_cmdline((struct multiboot_tag_string *)tag);
      break;
    case MULTIBOOT_TAG_TYPE_MMAP:
      boot_params_init_multiboot2_mmap((struct multiboot_tag_mmap *)tag);
      break;
    }
}

void boot_params_init(uint32_t magic, uint32_t addr)
{
  KASSERT(magic == MULTIBOOT2_BOOTLOADER_MAGIC);
  boot_params_init_multiboot2((struct multiboot_boot_information *)addr);
}

