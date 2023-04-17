#include "alloc.h"

#include "third_party/liballoc_1_1.h"

void *boot_malloc(size_t size)             { return internal_malloc(size); }
void *boot_realloc(void *ptr, size_t size) { return internal_realloc(ptr, size); }
void boot_kfree(void *ptr)                 { internal_free(ptr); }
