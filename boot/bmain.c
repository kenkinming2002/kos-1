#include "boot_params.h"

#include "core/debug.h"

#include "debug.h"
#include "mmap.h"

void bmain(uint32_t magic, struct multiboot_boot_information *boot_info)
{
  debug_init();
  debug_printf("hello magic=0x%lx, addr=0x%lx\n", magic, (uintptr_t)boot_info);

  boot_params_init(magic, (uintptr_t)boot_info);

  mmap_init(boot_info);
  for(size_t i=0; i<mmap.count; ++i)
    debug_printf(" => addr=0x%lx, length=0x%lx, type=%u\n",
        mmap.entries[i].addr,
        mmap.entries[i].length,
        mmap.entries[i].type);
}
