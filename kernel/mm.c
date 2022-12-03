#include "mm.h"

void mm_init(struct boot_service *service)
{
  mm_init_pages(service);
}
