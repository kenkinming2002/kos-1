#ifndef BOOT_FILES_H
#define BOOT_FILES_H

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

extern struct file files[MAX_FILES];
extern size_t      file_count;

void files_init(struct multiboot_boot_information *boot_info);

#endif // BOOT_FILES_H

