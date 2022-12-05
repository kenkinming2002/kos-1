#include "core/assert.h"
#include "core/debug.h"

#include "debug.h"
#include "mm.h"
#include "pal.h"
#include "hal.h"
#include "irqs.h"

#include "timer/pit.h"

#include <boot/service.h>

#include <stdint.h>

#define TEST_COUNT 10

DEFINE_MODULE(dummy);

void handle_device_not_available()
{
  debug_printf("device not available\n");
}

void handle_timer()
{
  debug_printf("timer interrupt\n");
}

void kmain(struct boot_service *service)
{
  debug_init();
  debug_printf("hello\n");

  mm_init(service);
  pal_init();
  hal_init();
  irqs_init();

  pit_init();

  struct timer *timer = timer_alloc();
  KASSERT(timer);
  timer->configure(timer, TIMER_MODE_PERIODIC, 100, &handle_timer, NULL);

  asm volatile ("int $0x7");
  asm volatile ("int $0x80");
  asm volatile ("sti");

  for(;;) asm volatile("hlt");
}
