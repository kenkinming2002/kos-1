#include "timer.h"

LL_DEFINE(timer_list);

void timer_register(struct timer *timer)
{
  ll_append(&timer_list, &timer->node);
}

void timer_deregister(struct timer *timer)
{
  ll_delete(&timer->node);
}
