#ifndef BOOT_INFO_H
#define BOOT_INFO_H

#include <stddef.h>
#include <stdint.h>

struct boot_info_mmap_entry
{
  uintptr_t addr;
  size_t    size;
};

struct boot_info_mmap
{
  size_t count;
  struct boot_info_mmap_entry entries[];
};

struct boot_info_alloc
{
  uint32_t boot_memory_begin;
  uint32_t boot_memory_end;
};

struct boot_info
{
  struct boot_info_mmap  *mmap;
  struct boot_info_alloc *alloc;
};

struct multiboot_boot_information *multiboot_boot_information_get(uint32_t addr);
struct boot_info_mmap *prepare_boot_info_mmap(struct multiboot_boot_information *mb_boot_info);
struct boot_info_alloc *prepare_boot_info_alloc();
struct boot_info *prepare_boot_info(struct multiboot_boot_information *mb_boot_info);

#endif // BOOT_INFO_H
