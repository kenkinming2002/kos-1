#ifndef INFO_ALL_H
#define INFO_ALL_H

#include "alloc.h"
#include "info.h"
#include "mmap.h"
#include "multiboot2_extra.h"

typedef struct kboot_info *kboot_info_t;
void kboot_init(struct multiboot_boot_information *mbi, kboot_info_t *kboot_info);

#endif // INFO_ALL_H
