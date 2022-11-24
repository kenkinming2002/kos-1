#include "pages.h"

int liballoc_lock() { }
int liballoc_unlock() { }

void* liballoc_alloc(size_t count)
{
  return alloc_pages(count);
}

/** This frees previously allocated memory. The void* parameter passed
 * to the function is the exact same value returned from a previous
 * liballoc_alloc call.
 *
 * The integer value is the number of pages to free.
 *
 * \return 0 if the memory was successfully freed.
 */
int liballoc_free(void* ptr, size_t count)
{
  free_pages(ptr, count);
  return 0;
}
