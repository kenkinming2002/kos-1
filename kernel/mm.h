#ifndef MM_H
#define MM_H

#include <boot/service.h>

#define ALLOC_FAILED (void*)(-1);

void mm_init(struct boot_service *boot_service);

void *alloc_pages(unsigned count);
void free_pages(void *pages, unsigned count);

#endif // MM_H
