#include "boot_params.h"

#include "core/debug.h"
#include "debug.h"

static void debug_print_mem_region(const char * name, struct mem_region *region)
{
  debug_printf("region: %s\n", name);
  for(size_t i=0; i<region->count; ++i)
    debug_printf(" => begin=0x%lx, end=0x%lx\n", region->areas[i].begin, region->areas[i].end);
}

void bmain(uint32_t magic, uint32_t addr)
{
  debug_init();
  debug_printf("hello magic=0x%lx, addr=0x%lx\n", magic, addr);

  boot_params_init(magic, addr);
  debug_print_mem_region("usable",           &boot_params.mmap.usable);
  debug_print_mem_region("acpi reclaimable", &boot_params.mmap.acpi_reclaimable);
  debug_print_mem_region("acpi nvs",         &boot_params.mmap.acpi_nvs);
  debug_print_mem_region("reserved",         &boot_params.mmap.reserved);
  debug_print_mem_region("bad",              &boot_params.mmap.bad);
}
