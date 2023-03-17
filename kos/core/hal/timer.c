#include "timer.h"

#include <stddef.h>

struct ll timer_list = LL_INIT(timer_list);

void timer_add(struct timer *timer)
{
  ll_append(&timer_list, &timer->node);
}

void timer_del(struct timer *timer)
{
  ll_delete(&timer->node);
}

struct timer *timer_alloc()
{
  LL_FOREACH(timer_list, node)
  {
    struct timer *timer = (struct timer *)node;
    ll_delete(node);
    return timer;
  }
  return NULL;
}

void timer_free(struct timer *timer)
{
  ll_append(&timer_list, &timer->node);
}
