#include "mm/all.h"
#include "arch/all.h"

#include "hal/module.h"
#include "hal/timer.h"
#include "hal/irq.h"

#include <arch/once.h>
#include <core/assert.h>
#include <core/log.h>

#include <limits.h>
#include <stdint.h>

#define TEST_COUNT 10

static void early_init(struct kboot_info *boot_info)
{
  static struct once once;
  if(once_begin(&once, ONCE_SYNC))
  {
    // Initialize
    mm_init(boot_info);
    irq_init();

    once_end(&once, ONCE_SYNC);
  }
}

bool on_device_not_available(struct slot *)
{
  logf("device not available\n");
  return false;
}

bool on_tick(struct slot *)
{
  logf("on tick\n");
  return false;
}

static struct slot_ops on_device_not_available_slot_ops = { .on_emit = &on_device_not_available, };
static struct slot_ops on_tick_slot_ops                 = { .on_emit = &on_tick, };

static struct slot on_device_not_available_slot;
static struct slot on_tick_slot;

// TODO: postpone these initialization task to the first schedulable process kinit
static void kinit()
{
  module_init();

  struct timer *timer = timer_alloc();
  KASSERT(timer);
  timer->configure(timer, TIMER_MODE_PERIODIC);
  timer->reload(timer, 64000000);

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
  logf("hello world {X:llu}\n", (long long unsigned)ULLONG_MAX);

  early_init(boot_info);
  arch_init();

  // Hacky way to restrict to a single core until until we have a proper
  // scheduler implementation
  static struct once once;
  if(!once_begin(&once, 0))
    for(;;)
      asm volatile ("hlt");

  kinit();
}

