#ifndef MEMORY_H
#define MEMORY_H

#include "pages.h"
#include "malloc.h"

void memory_init(struct kboot_info *boot_info);

void *kmalloc(size_t size);
void *krealloc(void *ptr, size_t size);
void *kcalloc(size_t nmemb, size_t size);
void kfree(void *ptr);

#endif // MEMORY_H
