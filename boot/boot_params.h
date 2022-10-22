#ifndef BOOT_PARAMS_H
#define BOOT_PARAMS_H

#include <stdint.h>
#include <stddef.h>

#define MAX_CMDLINE_LENGTH 256
#define MAX_MEM_AREA_COUNT 32

struct mem_area
{
  uintptr_t addr;
  size_t    length;
};

struct mem_region
{
  struct mem_area areas[MAX_MEM_AREA_COUNT];
  size_t          count;
};

struct mmap
{
  struct mem_region usable;

  struct mem_region acpi_reclaimable;
  struct mem_region acpi_nvs;

  struct mem_region reserved;
  struct mem_region bad;
};

struct boot_params
{
  char cmdline[MAX_CMDLINE_LENGTH];
  struct mmap mmap;
};
extern struct boot_params boot_params;

void boot_params_init(uint32_t magic, uint32_t addr);

#endif // BOOT_PARAMS_H
