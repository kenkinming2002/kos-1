#include <boot/service.h>

struct boot_service boot_service = {
  .mmap_iterate     = NULL,
  .mmap_alloc_pages = NULL,
  .mmap_free_pages  = NULL,
  .fs_iterate       = NULL,
  .fs_read          = NULL,
  .fs_write         = NULL,
};

