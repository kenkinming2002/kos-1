#include "pit.h"

#include "mm.h"
#include "hal.h"
#include "irqs.h"

#include <core/assert.h>
#include <core/ll.h>

#define PIT_PORTS      0x40
#define PIT_PORT_COUNT 4

#define PIT_CHANNEL0 (PIT_PORTS + 0)
#define PIT_CHANNEL1 (PIT_PORTS + 1)
#define PIT_CHANNEL2 (PIT_PORTS + 2)
#define PIT_MODE_COMMAND_REGISTER (PIT_PORTS + 3)

enum pit_select_channel
{
  PIT_SELECT_CHANNEL0         = 0b00000000,
  PIT_SELECT_CHANNEL1         = 0b01000000,
  PIT_SELECT_CHANNEL2         = 0b10000000,
  PIT_SELECT_READBACK_COMMAND = 0b11000000,
};

enum pit_access_mode
{
  PIT_ACCESS_LOBYTE_ONLY   = 0b00010000,
  PIT_ACCESS_HIBYTE_ONLY   = 0b00100000,
  PIT_ACCESS_LOBYTE_HIBYTE = 0b00110000,
};

enum pit_operating_mode
{
  PIT_INTERRUPT_ON_TERMINAL_COUNT    = 0b00000000,
  PIT_HARDWARE_RETRIGGERABLE_ONESHOT = 0b00000010,
  PIT_RATE_GENERATOR1                = 0b00000100,
  PIT_SQUARE_WAVE_GENERATOR1         = 0b00000110,

  PIT_SOFTWARE_TRIGGERED_STROBE      = 0b00001000,
  PIT_HARDWARE_TRIGGERED_STROBE      = 0b00001010,
  PIT_RATE_GENERATOR2                = 0b00001100,
  PIT_SQUARE_WAVE_GENERATOR2         = 0b00001110,
};

#define PIT_BCD 0b00000001;

DEFINE_MODULE(pit);

struct timer_record
{
  struct ll_node node;

  struct module *module;
  timer_callback_t callback;
};
static LL_DEFINE(timer_records);

static void pit_handler(void *)
{
  LL_FOREACH(timer_records, node)
  {
    struct timer_record *record = (struct timer_record *)node;
    record->callback();
  }
}

void pit_init()
{
  KASSERT(acquire_ports(THIS_MODULE, PIT_PORTS, PIT_PORT_COUNT) != -1);
  outb(PIT_MODE_COMMAND_REGISTER, PIT_SELECT_CHANNEL0 | PIT_ACCESS_LOBYTE_HIBYTE | PIT_RATE_GENERATOR1);
  outb(PIT_SELECT_CHANNEL0, 0xFF);
  outb(PIT_SELECT_CHANNEL0, 0xFF);

  KASSERT(isa_irq_register(THIS_MODULE, 0, &pit_handler, NULL) != -1);
}

int pit_register_callback(struct module *module, timer_callback_t callback)
{
  struct timer_record *record = kmalloc(sizeof *record);
  record->module   = module;
  record->callback = callback;
  ll_append(&timer_records, &record->node);
  return 0;
}

int pit_deregister_callback(struct module *module, timer_callback_t callback)
{
  LL_FOREACH(timer_records, node)
  {
    struct timer_record *record = (struct timer_record *)node;
    if(record->module == module && record->callback == callback)
    {
      ll_delete(node);
      kfree(record);
      return 0;
    }
  }
  return -1;
}

