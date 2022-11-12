#include <boot/service.h>

struct boot_service boot_service = {
  .mm_iterate     = NULL,
  .mm_alloc_pages = NULL,
  .mm_free_pages  = NULL,
  .fs_iterate       = NULL,
  .fs_read          = NULL,
  .fs_write         = NULL,
};

