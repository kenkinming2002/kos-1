#include <boot/service.h>

#include "mm.h"

struct boot_service boot_service = {
  .mm_iterate     = boot_mm_iterate,
  .mm_alloc_pages = boot_mm_alloc_pages,
  .mm_free_pages  = boot_mm_free_pages,
  .fs_iterate     = NULL,
  .fs_read        = NULL,
  .fs_write       = NULL,
};

