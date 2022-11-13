#ifndef BOOT_FS_H
#define BOOT_FS_H

#include <boot/service.h>

#include <stddef.h>
#include <stdint.h>

#include "multiboot2_extra.h"

void fs_init(struct multiboot_boot_information *boot_info);

void boot_fs_iterate(void(*)(struct boot_file *file));
void boot_fs_read(struct boot_file *file, size_t offset, size_t length, char *buffer);
void boot_fs_write(struct boot_file *file, size_t offset, size_t length, const char *buffer);


#endif // BOOT_FS_H

