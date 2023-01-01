#include <kcore/assert.h>
#include <kcore/debug.h>

#include "debug.h"
#include "kboot/all.h"
#include "kernel.h"

void bmain(uint64_t /*magic*/, struct multiboot_boot_information *boot_info)
{
  entry_t      entry;
  kboot_info_t kboot_info;

  debug_init();
  load_kernel(boot_info, &entry);
  kboot_init(boot_info, &kboot_info);

  entry(kboot_info);
  KASSERT_UNREACHABLE;
}
