#ifndef BOOT_MM_H
#define BOOT_MM_H

#include <stddef.h>
#include <stdint.h>

#include "multiboot2_extra.h"

enum MemoryType
{
  MEMORY_AVAILABLE,

  MEMORY_BOOTLOADER_RECLAIMABLE, /* Memory used by bootloader */
  MEMORY_FILESYSTEM,

  MEMORY_ACPI_RECLAIMABLE,
  MEMORY_ACPI_NVS,

  MEMORY_RESERVED,

  MEMORY_BAD,
};

struct mmap_entry
{
  uintptr_t addr;
  size_t    length;

  enum MemoryType type;
};

#define MAX_MMAP_ENTRIES 32
struct mmap
{
  struct mmap_entry entries[MAX_MMAP_ENTRIES];
  size_t            count;
};
extern struct mmap mmap;

void mm_init(struct multiboot_boot_information *boot_info);
void *mm_alloc(size_t count);

#endif // BOOT_MM_H
