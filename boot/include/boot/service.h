#ifndef BOOT_SERVICE_H
#define BOOT_SERVICE_H

#include <stddef.h>
#include <stdint.h>

#define PATH_MAX 256

// Memory Services
enum boot_memory_type
{
  BOOT_MEMORY_AVAILABLE,

  BOOT_MEMORY_BOOTLOADER_RECLAIMABLE,
  BOOT_MEMORY_BOOTLOADER_FILESYSTEM,
  BOOT_MEMORY_BOOTLOADER_ALLOCATED,

  BOOT_MEMORY_ACPI_RECLAIMABLE,
  BOOT_MEMORY_ACPI_NVS,

  BOOT_MEMORY_RESERVED,
  BOOT_MEMORY_BAD,
};

struct boot_mmap_entry
{
  enum boot_memory_type type;
  uintptr_t             addr;
  size_t                length;
};

typedef void(*boot_mm_iterate_t)(void(*)(struct boot_mmap_entry *mmap_entry));
typedef void *(*boot_mm_alloc_pages_t)(size_t count);
typedef void (*boot_mm_free_pages_t)(void *ptr, size_t count);

// Filesystem Services
struct boot_file;

typedef void(*boot_fs_iterate_t)(void(*)(struct boot_file *file));

typedef const char *(*boot_fs_get_name_t)  (struct boot_file *file);
typedef size_t      (*boot_fs_get_length_t)(struct boot_file *file);

typedef void(*boot_fs_read_t) (struct boot_file *file, size_t offset, size_t length, char *buffer);
typedef void(*boot_fs_write_t)(struct boot_file *file, size_t offset, size_t length, const char *buffer);

struct boot_service
{
  // Memory services
  boot_mm_iterate_t      mm_iterate;
  boot_mm_alloc_pages_t  mm_alloc_pages;
  boot_mm_free_pages_t   mm_free_pages;

  // Filesystem Services
  boot_fs_iterate_t fs_iterate;

  boot_fs_get_name_t   fs_get_name;
  boot_fs_get_length_t fs_get_length;

  boot_fs_read_t    fs_read;
  boot_fs_write_t   fs_write;
};

#endif // BOOT_SERVICE_H
