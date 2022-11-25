#ifndef BOOT_SERVICE_H
#define BOOT_SERVICE_H

#include <stddef.h>
#include <stdint.h>

#define PATH_MAX 256

// Memory Services
//
// Separate into -
//  type:   conventional, reserved, nvs, bad
//  owner:  none, bootloader, kernel, acpi
//
// Example(Mapping for multiboot2 memory type) -
//  available        => conventional + none
//  acpi reclaimable => conventional + acpi
//  nvs              => nvs          + acpi
//  reserved         => reserved     + none
//  bad              => bad          + none
//
// memory allocatable/usable by the pages allocator <=> type = conventional + owner = none
// memory allocatable/usable by the mmio layer      <=> type = reserved     + owner = none
//
// Type of memory cannot change, but ownership can transffer
//
enum boot_memory_type
{
  BOOT_MEMORY_CONVENTIONAL,
  BOOT_MEMORY_RESERVED,
  BOOT_MEMORY_NVS,
  BOOT_MEMORY_BAD,
};

enum boot_memory_owner
{
  BOOT_MEMORY_UNOWNED,
  BOOT_MEMORY_ACPI,
  BOOT_MEMORY_BOOTLOADER,
  BOOT_MEMORY_KERNEL,
};

struct boot_mmap_entry
{
  // TODO: Consider using bit mask or bit field
  enum boot_memory_type  type;
  enum boot_memory_owner owner;

  uintptr_t             addr;
  size_t                length;
};

typedef void(*boot_mm_iterate_t)(void(*)(struct boot_mmap_entry *mmap_entry));
typedef void *(*boot_mm_alloc_pages_t)(size_t count);
typedef void (*boot_mm_free_pages_t)(void *ptr, size_t count);

// Filesystem Services
struct boot_file
{
  const char *name;

  char   *data;
  size_t  length;
};

typedef void(*boot_fs_iterate_t)(void(*)(struct boot_file *file));
typedef struct boot_file *(*boot_fs_lookup_t)(const char *path);

typedef const char *(*boot_fs_get_name_t)  (struct boot_file *file);
typedef size_t      (*boot_fs_get_length_t)(struct boot_file *file);

struct boot_service
{
  // Memory services
  boot_mm_iterate_t      mm_iterate;
  boot_mm_alloc_pages_t  mm_alloc_pages;
  boot_mm_free_pages_t   mm_free_pages;

  // Filesystem Services
  boot_fs_iterate_t fs_iterate;
  boot_fs_lookup_t  fs_lookup;
};

#endif // BOOT_SERVICE_H
