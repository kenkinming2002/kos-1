#ifndef INITRD_H
#define INITRD_H

#include "multiboot2_extra.h"

#include <stddef.h>

void load_initrd(struct multiboot_boot_information *mbi, const char **data, size_t *length);

#endif // INITRD_H
