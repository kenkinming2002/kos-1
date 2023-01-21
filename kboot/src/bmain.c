#include "multiboot2_extra.h"
#include "debug.h"
#include "initrd.h"
#include "kernel.h"
#include "kboot/all.h"

#include <kcore/assert.h>

void bmain(uint64_t /*magic*/, struct multiboot_boot_information *boot_info)
{
  entry_t     entry;
  const char *initrd_data;
  size_t      initrd_length;

  kboot_info_t kboot_info;

  debug_init();
  load_kernel(boot_info, &entry);
  load_initrd(boot_info, &initrd_data, &initrd_length);
  kboot_init(boot_info, &kboot_info);

  entry(kboot_info);
  KASSERT_UNREACHABLE;
}
