#ifndef HAL_RES_H
#define HAL_RES_H

#include "module.h"

#include <stddef.h>

enum res_type
{
  RES_IRQ_BUS_ROOT_INPUT,
  RES_IRQ_BUS_ROOT_OUTPUT,

  RES_IRQ_BUS_ISA_INPUT,
  RES_IRQ_BUS_ISA_OUTPUT,

  RES_IRQ_BUS_GSI_INPUT,
  RES_IRQ_BUS_GSI_OUTPUT,

  RES_IOPORT,
  RES_MMIO,
};

int res_acquire(enum res_type type, struct module *module, size_t index, size_t count);
int res_release(enum res_type type, struct module *module, size_t index, size_t count);

#endif // HAL_RES_H
