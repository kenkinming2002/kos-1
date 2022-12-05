#ifndef HAL_TIMER_H
#define HAL_TIMER_H

enum timer_mode
{
  TIMER_MODE_ONESHOT,
  TIMER_MODE_PERIODIC,
};

typedef void(*timer_handler_t)(void *);
struct timer
{
  void(*configure)(enum timer_mode mode, unsigned duration, timer_handler_t handler, void *data);
};

#endif // HAL_TIMER_H
