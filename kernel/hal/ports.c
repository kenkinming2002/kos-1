#include "ports.h"

#include "mm/malloc.h"

#include <core/ll.h>

struct ports
{
  struct ll_node node;

  struct driver *driver;
  uint16_t begin;
  uint16_t count;
};

LL_DEFINE(ports_list);
int acquire_ports(struct driver *driver, uint16_t begin, uint16_t count)
{
  LL_FOREACH(ports_list, node)
  {
    struct ports *ports = (struct ports *)node;
    if(ports->begin + ports->count > begin && begin + count > ports->begin)
      return -1; // Overlap
  }

  struct ports *ports = kmalloc(sizeof *ports);
  ports->driver = driver;
  ports->begin  = begin;
  ports->count  = count;
  ll_append(&ports_list, &ports->node);
  return 0;
}

int release_ports(struct driver *driver, uint16_t begin, uint16_t count)
{
  LL_FOREACH(ports_list, node)
  {
    struct ports *ports = (struct ports *)node;
    if(ports->driver == driver && ports->begin == begin && ports->count == count)
    {
      ll_delete(&ports->node);
      kfree(ports);
      return 0;
    }
  }
  return -1;
}
