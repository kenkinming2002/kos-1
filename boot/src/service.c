#include <boot/service.h>

#include "mm.h"
#include "fs.h"

struct boot_service boot_service = {
  .mm_iterate     = boot_mm_iterate,
  .mm_alloc_pages = boot_mm_alloc_pages,
  .mm_free_pages  = boot_mm_free_pages,
  .fs_iterate     = boot_fs_iterate,
  .fs_read        = boot_fs_read,
  .fs_write       = boot_fs_write,
};

