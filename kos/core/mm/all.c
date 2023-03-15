#include "all.h"

void mm_init(struct kboot_info *boot_info)
{
  mm_init_pages(boot_info);
}
