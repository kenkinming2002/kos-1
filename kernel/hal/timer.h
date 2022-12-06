#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include <core/ll.h>

enum timer_mode
{
  TIMER_MODE_ONESHOT,
  TIMER_MODE_PERIODIC,
};

typedef void(*timer_handler_t)(void *);
struct timer
{
  struct ll_node node;

  void(*enable)(struct timer *timer);
  void(*disable)(struct timer *timer);

  void(*configure)(struct timer *timer, enum timer_mode mode, timer_handler_t handler, void *data);
  void(*reload)(struct timer *timer, unsigned duration);
};

void timer_register(struct timer *timer);
void timer_deregister(struct timer *timer);

struct timer *timer_alloc();
void timer_free(struct timer *timer);

#endif // HAL_TIMER_H
