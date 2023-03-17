#ifndef COER_HAL_TIMER_H
#define COER_HAL_TIMER_H

#include <core/ll.h>
#include <core/slot.h>

struct timer
{
  struct ll_node node;

  struct timer_ops *ops;
  struct slot       slot;
};

enum timer_mode
{
  TIMER_MODE_ONESHOT,
  TIMER_MODE_PERIODIC,
};

struct timer_ops
{
  void(*configure)(struct timer *timer, enum timer_mode mode);
  void(*reload)(struct timer *timer, unsigned duration);
};

void timer_add(struct timer *timer);
void timer_del(struct timer *timer);

struct timer *timer_alloc();
void timer_free(struct timer *timer);

#endif // CORE_HAL_TIMER_H
