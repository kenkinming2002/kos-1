#include "arch/all.h"

#include "core/mm/all.h"
#include "core/hal/module.h"
#include "core/hal/timer.h"
#include "core/hal/irq.h"

#include <rt/core/assert.h>
#include <rt/core/log.h>
#include <rt/core/once.h>

#include <limits.h>
#include <stdint.h>

#define TEST_COUNT 10

bool on_device_not_available(struct slot *)
{
  logf(LOG_WARN "device not available\n");
  return false;
}

bool on_tick(struct slot *)
{
  logf(LOG_TRACE "on tick\n");
  return false;
}

static struct slot_ops on_device_not_available_slot_ops = { .on_emit = &on_device_not_available, };
static struct slot_ops on_tick_slot_ops                 = { .on_emit = &on_tick, };

static struct slot on_device_not_available_slot;
static struct slot on_tick_slot;

// TODO: postpone these initialization task to the first schedulable process kinit
static void kinit(struct kboot_info *boot_info)
{
  mm_init(boot_info);
  irq_init();
  module_init();

  struct timer *timer = timer_alloc();
  KASSERT(timer);
  timer->ops->configure(timer, TIMER_MODE_PERIODIC);
  timer->ops->reload(timer, 64000000);

  // Initialize slots
  slot_init(&on_device_not_available_slot);
  slot_init(&on_tick_slot);
  on_device_not_available_slot.name = "main:on device not available";
  on_tick_slot                .name = "main:on tick";
  on_device_not_available_slot.ops  = &on_device_not_available_slot_ops;
  on_tick_slot                .ops  = &on_tick_slot_ops;

  // Connect slots
  slot_connect(irq_slot(IRQ_BUS_EXCEPTION, 7), &on_device_not_available_slot);
  slot_connect(&timer->slot,                   &on_tick_slot);

  slot_activate(&on_device_not_available_slot);
  slot_activate(&on_tick_slot);

  asm volatile ("int $0x7");
  asm volatile ("int $0x80");
  asm volatile ("sti");

  for(;;) asm volatile("hlt");
}


void kmain(struct kboot_info *boot_info)
{
  arch_init();

  // Hacky way to restrict to a single core until until we have a proper
  // scheduler implementation
  static struct once once;
  if(!once_begin(&once, 0))
    for(;;)
      asm volatile ("hlt");

  kinit(boot_info);
}

