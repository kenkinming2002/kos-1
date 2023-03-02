#include "debug.h"
#include "mm/all.h"
#include "arch/all.h"
#include "hal/all.h"
#include "hal/module.h"
#include "dev/all.h"

#include <arch/once.h>
#include <core/assert.h>
#include <core/debug.h>

#include <stdint.h>

#define TEST_COUNT 10

static void early_init(struct kboot_info *boot_info)
{
  static struct once once;
  if(once_begin(&once, ONCE_SYNC))
  {
    // Initialize
    debug_init();
    mm_init(boot_info);
    hal_init();

    once_end(&once, ONCE_SYNC);
  }
}

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

// TODO: postpone these initialization task to the first schedulable process kinit
static void kinit()
{
  extern struct module module_begin[];
  extern struct module module_end[];

  for(struct module *module = module_begin; module != module_end; ++module)
    module_register(module);

  module_update();

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

void kmain(struct kboot_info *boot_info)
{
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

