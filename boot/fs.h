#ifndef BOOT_FS_H
#define BOOT_FS_H

#include <stddef.h>
#include <stdint.h>

#include "multiboot2_extra.h"

#define MAX_FILES           32
#define MAX_FILENAME_LENGTH 256

struct file
{
  char name[MAX_FILENAME_LENGTH];

  uintptr_t addr;
  size_t    length;
};

struct fs
{
  struct file files[MAX_FILES];
  size_t      count;
};

extern struct fs fs;
void fs_init(struct multiboot_boot_information *boot_info);

#endif // BOOT_FS_H

