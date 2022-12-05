#include "pit.h"

#include "mm.h"
#include "hal.h"
#include "irqs.h"

#include <core/assert.h>
#include <core/ll.h>

DEFINE_MODULE(pit);

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

struct pit
{
  struct timer timer;

  timer_handler_t  handler;
  void            *data;
};

static void pit_handler(void *data)
{
  struct pit *pit = data;
  pit->handler(pit->data);
}

static uint64_t _pit_reload_value_from_duration(uint64_t duration)
{
  // Duration specified in ns
  return duration * 1193192 / 1000000000;
}

static uint16_t pit_reload_value_from_duration(unsigned duration)
{
  uint64_t reload_value = _pit_reload_value_from_duration(duration);
  if(reload_value >= UINT16_MAX)
    reload_value = UINT16_MAX;

  if(reload_value == 0)
    reload_value = 1;

  return reload_value;
}

static void pit_enable(struct timer *timer)
{
  struct pit *pit = (struct pit *)timer;
  KASSERT(isa_irq_register(THIS_MODULE, 0, pit_handler, pit) == 0);
}

static void pit_disable(struct timer *timer)
{
  struct pit *pit = (struct pit *)timer;
  KASSERT(isa_irq_deregister(THIS_MODULE, 0) == 0);
}

static void pit_configure(struct timer *timer, enum timer_mode mode, unsigned duration, timer_handler_t handler, void *data)
{
  struct pit *pit = (struct pit *)timer;
  outb(PIT_MODE_COMMAND_REGISTER, PIT_SELECT_CHANNEL0 | PIT_ACCESS_LOBYTE_HIBYTE | PIT_RATE_GENERATOR1);

  uint16_t reload_value = pit_reload_value_from_duration(duration);
  outb(PIT_SELECT_CHANNEL0, (reload_value >> 0) & 0xFF);
  outb(PIT_SELECT_CHANNEL0, (reload_value >> 8) & 0xFF);

  pit->handler = handler;
  pit->data    = data;
}

static struct pit *pit_create()
{
  struct pit *pit = kmalloc(sizeof *pit);
  pit->timer.enable    = &pit_enable;
  pit->timer.disable   = &pit_disable;
  pit->timer.configure = &pit_configure;
  KASSERT(acquire_ports(THIS_MODULE, PIT_PORTS, PIT_PORT_COUNT) == 0);
  return pit;
}

static void pit_destroy(struct pit *pit)
{
  KASSERT(release_ports(THIS_MODULE, PIT_PORTS, PIT_PORT_COUNT) == 0);
  kfree(pit);
}

void pit_init()
{
  struct pit *pit = pit_create();
  timer_register(&pit->timer);
}

