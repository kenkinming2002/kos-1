#ifndef BOOT_MMAP_H
#define BOOT_MMAP_H

#include <stddef.h>
#include <stdint.h>

#include "multiboot2_extra.h"

enum MemoryType
{
  MEMORY_AVAILABLE,

  MEMORY_ACPI_RECLAIMABLE,
  MEMORY_ACPI_NVS,

  MEMORY_RESERVED,
  MEMORY_BAD,
};

struct mmap_entry
{
  // Note: Includes the byte pointed to by end.
  // 1: The APIC mmio region is at the end of 4GiB address space, so there is
  //    no address pointing to the bytes 1 passed it.
  // 2: It is pointless to store empty area anyway.
  uintptr_t addr;
  size_t    length;

  enum MemoryType type;
};

#define MAX_MMAP_ENTRIES 32

extern struct mmap_entry mmap_entries[MAX_MMAP_ENTRIES];
extern size_t            mmap_entry_count;

void mmap_init(struct multiboot_boot_information *boot_info);

#endif // BOOT_MMAP_H
