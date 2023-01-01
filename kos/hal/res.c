#include "res.h"

#include "mm/all.h"

#include <kcore/assert.h>
#include <kcore/ll.h>

#include <stdint.h>

struct res
{
  struct ll_node node;

  struct module *module;
  size_t base, limit;
};

struct res_pool
{
  struct ll res_list;
  size_t limit; // 1 less than size
};

#define DEFINE_RES_POOL(name, limit) struct res_pool name##_pool = { LL_INIT(name##_pool.res_list), limit }

#define IRQ_BUS_ROOT_COUNT 256
#define IRQ_BUS_ISA_COUNT  16
#define IRQ_BUS_GSI_COUNT  0 // TODO: Maybe determine this dynamically?

DEFINE_RES_POOL(irq_bus_root_input,  IRQ_BUS_ROOT_COUNT - 1);
DEFINE_RES_POOL(irq_bus_root_output, IRQ_BUS_ROOT_COUNT - 1);
DEFINE_RES_POOL(irq_bus_isa_input,   IRQ_BUS_ISA_COUNT - 1);
DEFINE_RES_POOL(irq_bus_isa_output,  IRQ_BUS_ISA_COUNT - 1);
DEFINE_RES_POOL(irq_bus_gsi_input,   IRQ_BUS_GSI_COUNT - 1);
DEFINE_RES_POOL(irq_bus_gsi_output,  IRQ_BUS_GSI_COUNT - 1);
DEFINE_RES_POOL(ioport, UINT16_MAX);
DEFINE_RES_POOL(mmio,   SIZE_MAX);

static struct res_pool *res_pool_get(enum res_type type)
{
  switch(type)
  {
  case RES_IRQ_BUS_ROOT_INPUT:  return &irq_bus_root_input_pool;
  case RES_IRQ_BUS_ROOT_OUTPUT: return &irq_bus_root_output_pool;
  case RES_IRQ_BUS_ISA_INPUT:   return &irq_bus_isa_input_pool;
  case RES_IRQ_BUS_ISA_OUTPUT:  return &irq_bus_isa_output_pool;
  case RES_IRQ_BUS_GSI_INPUT:   return &irq_bus_gsi_input_pool;
  case RES_IRQ_BUS_GSI_OUTPUT:  return &irq_bus_gsi_output_pool;
  case RES_IOPORT:          return &ioport_pool;
  case RES_MMIO:            return &mmio_pool;
  default:
    KASSERT_UNREACHABLE;
  }
}

static int _res_acquire(enum res_type type, struct module *module, size_t base, size_t limit)
{
  struct res_pool *pool = res_pool_get(type);
  LL_FOREACH(pool->res_list, node)
  {
    struct res *res = (struct res *)node;
    if(base <= res->base + res->limit && res->base <= base + limit)
      return -1; // Allocated
  }

  struct res *res = kmalloc(sizeof *res);
  res->module = module;
  res->base   = base;
  res->limit  = limit;
  ll_append(&pool->res_list, &res->node);
  return 0;
}

static void _res_release(enum res_type type, struct module *module, size_t base, size_t limit)
{
  struct res_pool *pool = res_pool_get(type);
  LL_FOREACH(pool->res_list, node)
  {
    struct res *res = (struct res *)node;
    if(module == res->module && base == res->base && limit == res->limit)
    {
      ll_delete(&res->node);
      kfree(res);
      return;
    }
  }
  KASSERT_UNREACHABLE;
}

int res_acquire(enum res_type type, struct module *module, size_t index, size_t count)
{
  KASSERT(count != 0);
  return _res_acquire(type, module, index, count - 1);
}

void res_release(enum res_type type, struct module *module, size_t index, size_t count)
{
  KASSERT(count != 0);
  _res_release(type, module, index, count - 1);
}
