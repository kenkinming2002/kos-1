#ifndef INFO_ALL_H
#define INFO_ALL_H

#include "alloc.h"
#include "info.h"
#include "mmap.h"
#include "multiboot2_extra.h"

extern struct kboot_info *info;
void kboot_init(struct multiboot_boot_information *mbi);

#endif // INFO_ALL_H
