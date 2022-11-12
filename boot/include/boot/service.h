#ifndef BOOT_SERVICE_H
#define BOOT_SERVICE_H

#define PATH_MAX 256

// Memory Services
struct boot_mmap_entry
{
  uintptr_t addr;
  size_t    length;
};

typedef void(*boot_mmap_iterate_t)(void(*)(struct boot_mmap_entry *mmap_entry));
typedef void *(*boot_mmap_alloc_pages_t)(size_t count);
typedef void (*boot_mmap_free_pages_t)(void *ptr, size_t count);

// Filesystem Services
struct boot_file;

typedef void(boot_fs_iterate_t)(void(*)(struct file *file));
typedef void(*boot_fs_read_t) (struct file *file, size_t offset, size_t length, char *buffer);
typedef void(*boot_fs_write_t)(struct file *file, size_t offset, size_t length, const char *buffer);

struct boot_service
{
  // Memory services
  boot_mmap_iterate_t      mmap_iterate;
  boot_mmap_alloc_pages_t  mmap_alloc_pages;
  boot_mmap_free_pages_t   mmap_free_pages;

  // Filesystem Services
  boot_fs_iterate_t fs_iterate;
  boot_fs_read_t    fs_read;
  boot_fs_write_t   fs_write;
};

#endif // BOOT_SERVICE_H
