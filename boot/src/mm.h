#ifndef BOOT_MM_H
#define BOOT_MM_H

#include <stddef.h>
#include <stdint.h>

#include <boot/service.h>

#include "multiboot2_extra.h"

void mm_init(struct multiboot_boot_information *boot_info);

void boot_mm_iterate(void(*)(struct boot_mmap_entry *mmap_entry));
uintptr_t boot_mm_lookup(size_t length, size_t alignment, enum boot_memory_type type, enum boot_memory_owner owner);
void boot_mm_transfer(uintptr_t addr, size_t length, enum boot_memory_type type, enum boot_memory_owner from, enum boot_memory_owner to);

void* boot_mm_alloc(size_t size, size_t alignment);
void boot_mm_free(void *ptr, size_t size);

#endif // BOOT_MM_H
