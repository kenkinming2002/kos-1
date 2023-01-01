#include "alloc.h"

#include "config.h"

__attribute__((section(".kboot"))) char kboot_area[KBOOT_MAX];
static size_t kboot_size;

void *kboot_alloc(size_t size)
{
  void *ptr = &kboot_area[kboot_size];
  kboot_size += size;
  return ptr;
}

