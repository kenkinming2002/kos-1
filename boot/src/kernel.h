#ifndef BOOT_KERNEL_H
#define BOOT_KERNEL_H

#include "multiboot2_extra.h"

void load_kernel(struct multiboot_boot_information *mbi);

#endif // BOOT_KERNEL_H
