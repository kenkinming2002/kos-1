#ifndef CORE_MM_PAGES_H
#define CORE_MM_PAGES_H

#include <kboot.h>

#define ALLOC_FAILED (void*)(-1);

void mm_init_pages(struct kboot_info *boot_info);

void *alloc_pages(unsigned count);
void free_pages(void *pages, unsigned count);

#endif // CORE_MM_PAGES_H
