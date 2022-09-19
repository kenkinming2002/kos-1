#ifndef BOOT_PARAMS_H
#define BOOT_PARAMS_H

#include <stdint.h>
#include <stddef.h>

#define MAX_CMDLINE_LENGTH 256
#define MAX_MMAP_ENTRIES   32

enum MemoryType
{
  MEMORY_AVAILABLE,
  MEMORY_RESERVED,
  MEMORY_ACPI_RECLAIMABLE,
  MEMORY_NVS,
  MEMORY_BADRAM,
};

struct mmap_entry
{
  uintptr_t       addr;
  size_t          length;
  enum MemoryType type;
};

struct boot_params
{
  char cmdline[MAX_CMDLINE_LENGTH];

  struct mmap_entry mmap_entries[MAX_MMAP_ENTRIES];
  size_t            mmap_entry_count;
};
extern struct boot_params boot_params;

void boot_params_init(uint32_t magic, uint32_t addr);

#endif // BOOT_PARAMS_H
