#ifndef KBOOT_H
#define KBOOT_H

#include <stdint.h>

#define KBOOT_CONVENTIONAL_MEMORY 0

#define KBOOT_SYSTEM_STRUCTURES   1
#define KBOOT_INITRD              2
#define KBOOT_KERNEL              3

#define KBOOT_RECLAIMABLE         4
#define KBOOT_ACPI_RECLAIMABLE    5

#define KBOOT_ACPI_NVS            6
#define KBOOT_RESERVED            7
#define KBOOT_BADRAM              8

struct kboot_mmap_entry
{
  uint64_t type;
  uint64_t begin;
  uint64_t end;
};

struct kboot_mmap
{
  struct kboot_mmap_entry *entries;
  uint64_t                 count;
};

struct kboot_fb
{
};

struct kboot_info
{
  struct kboot_mmap *mmap;
  struct kboot_fb   *fb;
};

#endif // KBOOT_H
