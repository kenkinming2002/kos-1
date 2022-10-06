#ifndef MM_LINEAR_H
#define MM_LINEAR_H

#include "defs.h"

static inline uintptr_t virt_to_phys(void *ptr)  { return (uintptr_t)ptr - LINEAR_MAP_BEGIN; }
static inline void *phys_to_virt(uintptr_t addr) { return (void *)(addr + LINEAR_MAP_BEGIN); }

#endif // MM_LINEAR_H
