#ifndef BOOT_PARAMS_H
#define BOOT_PARAMS_H

#include <stdint.h>
#include <stddef.h>

#define MAX_CMDLINE_LENGTH 256
#define MAX_MMAP_ENTRIES   32
#define MAX_FILES           32
#define MAX_FILENAME_LENGTH 256

// mmap
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

struct mmap
{
  struct mmap_entry entries[MAX_MMAP_ENTRIES];
  size_t            count;
};

// fs
struct file
{
  char name[MAX_FILENAME_LENGTH];

  uintptr_t addr;
  size_t    length;
};

struct fs
{
  struct file files[MAX_FILES];
  size_t      count;
};


struct boot_params
{
  char cmdline[MAX_CMDLINE_LENGTH];
  struct mmap mmap;
  struct fs   fs;
};
extern struct boot_params boot_params;

void boot_params_init(uint32_t magic, uint32_t addr);

#endif // BOOT_PARAMS_H
