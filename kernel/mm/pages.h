#ifndef PAGES_H
#define PAGES_H

#include <boot/service.h>

#define ALLOC_FAILED (void*)(-1);

void mm_init_pages(struct boot_service *boot_service);

void *alloc_pages(unsigned count);
void free_pages(void *pages, unsigned count);

#endif // PAGES_H
