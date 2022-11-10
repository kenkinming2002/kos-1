#ifndef BOOT_MM_H
#define BOOT_MM_H

#include <stddef.h>
#include <stdint.h>

#include "multiboot2_extra.h"

void mm_init(struct multiboot_boot_information *boot_info);
void *mm_alloc(size_t count);

#endif // BOOT_MM_H
