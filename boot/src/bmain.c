#include "core/debug.h"

#include "debug.h"
#include "kboot/all.h"
#include "kernel.h"

void bmain(uint64_t /*magic*/, struct multiboot_boot_information *boot_info)
{
  debug_init();
  kboot_init(boot_info);
  load_kernel(boot_info);
}
