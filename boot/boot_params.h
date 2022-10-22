#ifndef BOOT_PARAMS_H
#define BOOT_PARAMS_H

#include <stdint.h>
#include <stddef.h>

#define MAX_CMDLINE_LENGTH 256
#define MAX_MMAP_ENTRIES   32

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

struct mmap
{
  struct mmap_entry entries[MAX_MMAP_ENTRIES];
  size_t            count;
};

struct boot_params
{
  char cmdline[MAX_CMDLINE_LENGTH];
  struct mmap mmap;
};
extern struct boot_params boot_params;

void boot_params_init(uint32_t magic, uint32_t addr);

#endif // BOOT_PARAMS_H
