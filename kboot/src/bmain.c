#include "multiboot2_extra.h"
#include "debug.h"
#include "initrd.h"
#include "kernel.h"
#include "kboot/all.h"

#include <core/assert.h>

void bmain(struct multiboot_boot_information *boot_info)
{
  static entry_t      entry;
  static const char  *initrd_data;
  static size_t       initrd_length;
  static kboot_info_t kboot_info;

  static int once;
  static int ready;

  // Initialize only once
  if(!__atomic_exchange_n(&once, 1, __ATOMIC_RELAXED))
  {
    // Initialize
    debug_init();
    load_kernel(boot_info, &entry);
    load_initrd(boot_info, &initrd_data, &initrd_length);
    kboot_init(boot_info, &kboot_info);
    __atomic_store_n(&ready, 1, __ATOMIC_RELEASE);
  }
  else
  {
    // Wait for initialization
    while(!__atomic_load_n(&ready, __ATOMIC_ACQUIRE))
      asm volatile("pause");
  }

  entry(kboot_info);
}

