#ifndef BOOT_ALLOC_H
#define BOOT_ALLOC_H

#include <stddef.h>

void *alloc_heap(size_t size, size_t alignment);
void *alloc_memory(size_t size, size_t alignment);

#endif // BOOT_ALLOC_H
