#ifndef BOOT_PAGES_H
#define BOOT_PAGES_H

#include <stddef.h>

void pages_init();

void *alloc_pages(size_t count);

#endif // BOOT_PAGES_H
