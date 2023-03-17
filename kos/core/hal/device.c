#include "device.h"

struct ll device_list = LL_INIT(device_list);

void device_add(struct device *device)
{
  ll_append(&device_list, &device->node);
  if(device->ops && device->ops->reset) device->ops->reset(device);
  if(device->ops && device->ops->debug) device->ops->debug(device);
}

void device_del(struct device *device)
{
  if(device->ops && device->ops->debug) device->ops->debug(device);
  if(device->ops && device->ops->reset) device->ops->reset(device);
  ll_delete(&device->node);
}
