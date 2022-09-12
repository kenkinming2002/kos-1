#include "debug.h"
#include "info.h"

#include "core/print.h"

#include <stdint.h>
#include <stddef.h>

void bmain(uint32_t magic, uint32_t addr)
{
  debug_init();
  debug_printf("magic = %lx, addr = %lx\n", magic, addr);

  struct multiboot_boot_information *mb_boot_info = multiboot_boot_information_get(addr);
  struct boot_info *boot_info = prepare_boot_info(mb_boot_info);
  for(size_t i=0; i<boot_info->mmap->count; ++i)
  {
    debug_printf("addr = 0x%lx, size = 0x%lx\n",
        boot_info->mmap->entries[i].addr,
        boot_info->mmap->entries[i].size);
  }
}
