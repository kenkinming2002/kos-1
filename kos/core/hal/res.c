#include "res.h"

#include "core/mm/all.h"

#include <rt/core/assert.h>
#include <rt/core/ll.h>

#include <stdint.h>

struct res
{
  struct ll_node node;
  size_t base, limit;
};

struct res_pool
{
  struct ll res_list;
  size_t limit; // 1 less than size
};

#define DEFINE_RES_POOL(name, limit) struct res_pool name##_pool = { LL_INIT(name##_pool.res_list), limit }

DEFINE_RES_POOL(irq_vector, UINT8_MAX);
DEFINE_RES_POOL(ioport,     UINT16_MAX);
DEFINE_RES_POOL(mmio,       SIZE_MAX);

static struct res_pool *res_pool_get(enum res_type type)
{
  switch(type)
  {
  case RES_IRQ_VECTOR: return &irq_vector_pool;
  case RES_IOPORT:     return &ioport_pool;
  case RES_MMIO:       return &mmio_pool;
  default:
    KASSERT_UNREACHABLE;
  }
}

static int _res_acquire(enum res_type type, size_t base, size_t limit)
{
  struct res_pool *pool = res_pool_get(type);
  LL_FOREACH(pool->res_list, node)
  {
    struct res *res = (struct res *)node;
    if(base <= res->base + res->limit && res->base <= base + limit)
      return -1; // Allocated
  }

  struct res *res = kmalloc(sizeof *res);
  res->base   = base;
  res->limit  = limit;
  ll_append(&pool->res_list, &res->node);
  return 0;
}

static void _res_release(enum res_type type, size_t base, size_t limit)
{
  struct res_pool *pool = res_pool_get(type);
  LL_FOREACH(pool->res_list, node)
  {
    struct res *res = (struct res *)node;
    if(base == res->base && limit == res->limit)
    {
      ll_delete(&res->node);
      kfree(res);
      return;
    }
  }
  KASSERT_UNREACHABLE;
}

int res_acquire(enum res_type type, size_t index, size_t count)
{
  KASSERT(count != 0);
  return _res_acquire(type, index, count - 1);
}

void res_release(enum res_type type, size_t index, size_t count)
{
  KASSERT(count != 0);
  _res_release(type, index, count - 1);
}
