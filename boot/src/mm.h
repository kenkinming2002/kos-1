#ifndef BOOT_MM_H
#define BOOT_MM_H

#include <stddef.h>
#include <stdint.h>

#include <boot/service.h>

#include "multiboot2_extra.h"

void mm_init(struct multiboot_boot_information *boot_info);

void boot_mm_iterate(void(*)(struct boot_mmap_entry *mmap_entry));
void *boot_mm_alloc_pages(size_t count);
void boot_mm_free_pages(void *ptr, size_t count);

#endif // BOOT_MM_H
