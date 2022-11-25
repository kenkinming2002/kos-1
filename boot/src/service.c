#include <boot/service.h>

#include "mm.h"
#include "fs.h"

struct boot_service boot_service = {
  .mm_iterate     = boot_mm_iterate,
  .mm_lookup      = boot_mm_lookup,
  .mm_transfer    = boot_mm_transfer,
  .mm_alloc       = boot_mm_alloc,
  .mm_free        = boot_mm_free,
  .fs_iterate     = boot_fs_iterate,
  .fs_lookup      = boot_fs_lookup,
};

