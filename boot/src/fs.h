#ifndef BOOT_FS_H
#define BOOT_FS_H

#include <stddef.h>
#include <stdint.h>

#include "multiboot2_extra.h"

void fs_init(struct multiboot_boot_information *boot_info);

#endif // BOOT_FS_H

