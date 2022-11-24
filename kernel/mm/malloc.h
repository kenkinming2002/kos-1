#ifndef MM_MALLOC_H
#define MM_MALLOC_H

#include <stddef.h>

void *kmalloc(size_t size);
void *krealloc(void *ptr, size_t size);
void *kcalloc(size_t nmemb, size_t size);
void kfree(void *ptr);

#endif // MM_MALLOC_H
