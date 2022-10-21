#include "boot_params.h"

void bmain(uint32_t magic, uint32_t addr)
{
  boot_params_init(magic, addr);
  debug_printf("hello magic=0x%lx, addr=0x%lx\n", magic, addr);
}
