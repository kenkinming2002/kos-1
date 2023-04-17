#include "pages.h"

#include <stddef.h>

int liballoc_lock()   { return 0; }
int liballoc_unlock() { return 0; }

void* liballoc_alloc(size_t count)
{
  return pages_alloc(count);
}

int liballoc_free(void* ptr, size_t count)
{
  pages_free(ptr, count);
  return 0;
}
