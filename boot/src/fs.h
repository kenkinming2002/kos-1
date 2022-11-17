#ifndef BOOT_FS_H
#define BOOT_FS_H

#include <boot/service.h>

#include <stddef.h>
#include <stdint.h>

#include "multiboot2_extra.h"

void fs_init(struct multiboot_boot_information *boot_info);

void boot_fs_iterate(void(*)(struct boot_file *file));

#endif // BOOT_FS_H

