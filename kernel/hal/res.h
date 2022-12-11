#ifndef HAL_RES_H
#define HAL_RES_H

#include "module.h"

#include <stddef.h>

enum res_type
{
  RES_IRQ_VECTOR,
  RES_IOPORT,
  RES_MMIO,
};

int res_acquire(enum res_type type, struct module *module, size_t index, size_t count);
int res_release(enum res_type type, struct module *module, size_t index, size_t count);

#endif // HAL_RES_H
