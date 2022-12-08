#include "core/assert.h"
#include "core/debug.h"

#include "debug.h"
#include "mm.h"
#include "pal.h"
#include "hal.h"
#include "dev.h"

#include <boot/service.h>

#include <stdint.h>

#define TEST_COUNT 10

DEFINE_MODULE(dummy);

void handle_device_not_available()
{
  debug_printf("device not available\n");
}

void on_tick(struct slot *slot)
{
  debug_printf("on tick\n");
}

static struct slot_ops timer_slot_ops = { .on_emit = &on_tick, };
static struct slot     timer_slot     = { .name = "on tick", .ops = &timer_slot_ops };

void kmain(struct boot_service *service)
{
  debug_init();
  debug_printf("hello\n");

  mm_init(service);
  pal_init();
  hal_init();
  dev_init();

  struct timer *timer = timer_alloc();
  slot_connect(&timer->slot, &timer_slot);
  KASSERT(timer);
  timer->configure(timer, TIMER_MODE_PERIODIC);
  timer->reload(timer, 64000000);

  asm volatile ("int $0x7");
  asm volatile ("int $0x80");
  asm volatile ("sti");

  for(;;) asm volatile("hlt");
}
