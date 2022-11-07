#ifndef BOOT_MM_H
#define BOOT_MM_H

#include <stddef.h>
#include <stdint.h>

#include "multiboot2_extra.h"

enum MemoryType
{
  MEMORY_AVAILABLE,

  MEMORY_BOOTLOADER_RECLAIMABLE, /* Memory used by bootloader */

  MEMORY_ACPI_RECLAIMABLE,
  MEMORY_ACPI_NVS,

  MEMORY_RESERVED,
  MEMORY_KERNEL,
  MEMORY_INITRD,

  MEMORY_BAD,
};

struct mmap_entry
{
  uintptr_t addr;
  size_t    length;

  enum MemoryType type;
};

#define MAX_MMAP_ENTRIES 32

extern struct mmap_entry mmap_entries[MAX_MMAP_ENTRIES];
extern size_t            mmap_entry_count;

void mm_init(struct multiboot_boot_information *boot_info);
void *mm_alloc(size_t count);

#endif // BOOT_MM_H
