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

void on_device_not_available(struct slot *slot)
{
  debug_printf("device not available\n");
}

void on_tick(struct slot *slot)
{
  debug_printf("on tick\n");
}

static struct slot_ops device_not_available_slot_ops = { .on_emit = &on_device_not_available, };
static struct slot     device_not_available_slot     = { .name = "on device not available", .ops = &device_not_available_slot_ops };

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

  irqs_bus_set_output("exceptions", 7, &device_not_available_slot);

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
