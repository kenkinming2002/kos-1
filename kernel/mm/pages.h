#ifndef PAGES_H
#define PAGES_H

#include <boot/service.h>
#include <boot/kboot.h>

#define ALLOC_FAILED (void*)(-1);

void mm_init_pages(struct kboot_info *boot_info);

void *alloc_pages(unsigned count);
void free_pages(void *pages, unsigned count);

#endif // PAGES_H
