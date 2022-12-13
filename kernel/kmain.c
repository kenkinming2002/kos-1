#include "core/assert.h"
#include "core/debug.h"

#include "debug.h"
#include "mm/all.h"
#include "pal/all.h"
#include "hal/all.h"
#include "dev/all.h"

#include <boot/service.h>

#include <stdint.h>

#define TEST_COUNT 10

bool on_device_not_available(struct irq_slot *)
{
  debug_printf("device not available\n");
  return false;
}

bool on_tick(struct irq_slot *)
{
  debug_printf("on tick\n");
  return false;
}

static struct irq_slot_ops device_not_available_slot_ops = { .on_emit = &on_device_not_available, };
static struct irq_slot     device_not_available_slot     = IRQ_SLOT_INIT("main:on device not available", &device_not_available_slot_ops, NULL);

static struct irq_slot_ops on_tick_slot_ops = { .on_emit = &on_tick, };
static struct irq_slot     on_tick_slot     = IRQ_SLOT_INIT("main:on tick", &on_tick_slot_ops, NULL);

void kmain(struct boot_service *service)
{
  debug_init();
  debug_printf("hello\n");

  mm_init(service);
  pal_init();
  hal_init();
  dev_init();

  KASSERT(irq_bus_set_output(IRQ_BUS_EXCEPTIONS, 7, &device_not_available_slot) == 0);

  struct timer *timer = timer_alloc();
  irq_slot_connect(&timer->slot, &on_tick_slot);
  irq_slot_unmask(&on_tick_slot);
  KASSERT(timer);
  timer->configure(timer, TIMER_MODE_PERIODIC);
  timer->reload(timer, 64000000);

  asm volatile ("int $0x7");
  asm volatile ("int $0x80");
  asm volatile ("sti");

  for(;;) asm volatile("hlt");
}

