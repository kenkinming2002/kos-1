#ifndef BOOT_KERNEL_H
#define BOOT_KERNEL_H

#include "multiboot2_extra.h"
#include "kboot/all.h"

typedef void(*entry_t)(kboot_info_t);
void load_kernel(struct multiboot_boot_information *mbi, entry_t *entry);

#endif // BOOT_KERNEL_H
