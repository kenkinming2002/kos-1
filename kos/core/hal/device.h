#ifndef CORE_HAL_DEVICE_H
#define CORE_HAL_DEVICE_H

#include <core/ll.h>

struct device
{
  struct ll_node node;

  const char        *name;
  struct device_ops *ops;
};

struct device_ops
{
  void(*reset)(struct device *device);
  void(*debug)(struct device *device);
};

void device_add(struct device *device);
void device_del(struct device *device);

#endif // CORE_HAL_DEVICE_H
