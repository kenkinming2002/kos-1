#ifndef PAGES_H
#define PAGES_H

#include <kboot.h>

#define ALLOC_FAILED (void*)(-1);
#define PAGE_SIZE 0x1000

void pages_init(struct kboot_info *boot_info);

void *alloc_pages(unsigned count);
void free_pages(void *pages, unsigned count);

#endif // PAGES_H
