#include "alloc.h"

#include <stdint.h>

extern char heap_begin[];
extern char memory_begin[];

void *alloc_heap(size_t size, size_t alignment)
{
  static uintptr_t addr = (uintptr_t)heap_begin;
  addr = (addr + alignment - 1) / alignment * alignment;

  void *ptr = (void *)addr;
  addr += size;
  return ptr;
}

void *alloc_memory(size_t size, size_t alignment)
{
  static uintptr_t addr = (uintptr_t)memory_begin;
  addr = (addr + alignment - 1) / alignment * alignment;

  void *ptr = (void *)addr;
  addr += size;
  return ptr;
}
