#include "device.h"

#include <rt/core/log.h>

struct ll device_list = LL_INIT(device_list);

void device_add(struct device *device)
{
  logf(LOG_INFO "adding device - name = {:s}\n", device->name);

  ll_append(&device_list, &device->node);
  if(device->ops && device->ops->reset) device->ops->reset(device);
  if(device->ops && device->ops->debug) device->ops->debug(device);
}

void device_del(struct device *device)
{
  logf(LOG_INFO "deleting device - name = {:s}\n", device->name);

  if(device->ops && device->ops->debug) device->ops->debug(device);
  if(device->ops && device->ops->reset) device->ops->reset(device);
  ll_delete(&device->node);
}
