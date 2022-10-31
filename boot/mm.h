#ifndef BOOT_MM_H
#define BOOT_MM_H

#include <stddef.h>

void mm_init();

void *alloc_pages(size_t count);

#endif // BOOT_MM_H
