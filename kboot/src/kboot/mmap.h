#ifndef KBOOT_MMAP_H
#define KBOOT_MMAP_H

#include "multiboot2_extra.h"

#include <kboot.h>

struct kboot_mmap *mmap_init(struct multiboot_boot_information *mbi);

#endif // KBOOT_MMAP_H
