#ifndef ALLOC_H
#define ALLOC_H

#include "pages.h"

#include <stddef.h>

// We want two version
// 1: Does not take lock. Suitable for early calling
// 2: Take locks, useful in general
void *boot_malloc(size_t size);
void *boot_realloc(void *ptr, size_t size);
void boot_free(void *ptr);

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#endif // ALLOC_H
