#include "boot_params.h"

#include "core/debug.h"
#include "debug.h"

void bmain(uint32_t magic, uint32_t addr)
{
  debug_init();
  debug_printf("hello magic=0x%lx, addr=0x%lx\n", magic, addr);

  boot_params_init(magic, addr);
  for(size_t i=0; i<boot_params.mmap.count; ++i)
    debug_printf(" => addr=0x%lx, length=0x%lx, type=%u\n",
        boot_params.mmap.entries[i].addr,
        boot_params.mmap.entries[i].length,
        boot_params.mmap.entries[i].type);
}
