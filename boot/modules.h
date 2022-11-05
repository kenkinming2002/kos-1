#ifndef BOOT_MODULES_H
#define BOOT_MODULES_H

#include <stddef.h>
#include <stdint.h>

#include "multiboot2_extra.h"

struct module
{
  // Note: Includes the byte pointed to by end.
  // 1: The APIC mmio region is at the end of 4GiB address space, so there is
  //    no address pointing to the bytes 1 passed it.
  // 2: It is pointless to store empty area anyway.
  uintptr_t addr;
  size_t    length;
};

#define MAX_MODULES 32

extern struct module modules[MAX_MODULES];
extern size_t        module_count;

void modules_init(struct multiboot_boot_information *boot_info);

#endif // BOOT_MODULES_H

