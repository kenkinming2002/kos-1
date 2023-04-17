#ifndef PAGES_H
#define PAGES_H

#include <kboot.h>
#include <stddef.h>

#define ALLOC_FAILED (void*)(-1);
#define PAGE_SIZE 0x1000

void pages_init(struct kboot_info *boot_info);

void *pages_alloc(size_t count);
void pages_free(void *pages, size_t count);

#endif // PAGES_H
