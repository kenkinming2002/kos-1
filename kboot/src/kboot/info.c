#include "info.h"

#include "alloc.h"
#include "mmap.h"

struct kboot_info *info_init(struct multiboot_boot_information *mbi)
{
  struct kboot_info *info = kboot_alloc(sizeof *info);
  info->mmap = mmap_init(mbi);
  return info;
}
