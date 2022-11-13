#include "core/debug.h"

#include "debug.h"
#include "mm.h"
#include "fs.h"

void bmain(uint64_t magic, struct multiboot_boot_information *boot_info)
{
  debug_init();
  mm_init(boot_info);
  fs_init(boot_info);
}
