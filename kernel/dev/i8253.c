#include "i8253.h"

#include "mm.h"
#include "hal.h"

#include <core/assert.h>
#include <core/ll.h>

DEFINE_MODULE(i8253);

#define I8253_PORTS      0x40
#define I8253_PORT_COUNT 4

#define I8253_CHANNEL0 (I8253_PORTS + 0)
#define I8253_CHANNEL1 (I8253_PORTS + 1)
#define I8253_CHANNEL2 (I8253_PORTS + 2)
#define I8253_MODE_COMMAND_REGISTER (I8253_PORTS + 3)

enum i8253_select_channel
{
  I8253_SELECT_CHANNEL0         = 0b00000000,
  I8253_SELECT_CHANNEL1         = 0b01000000,
  I8253_SELECT_CHANNEL2         = 0b10000000,
  I8253_SELECT_READBACK_COMMAND = 0b11000000,
};

enum i8253_access_mode
{
  I8253_ACCESS_LOBYTE_ONLY   = 0b00010000,
  I8253_ACCESS_HIBYTE_ONLY   = 0b00100000,
  I8253_ACCESS_LOBYTE_HIBYTE = 0b00110000,
};

/* Different mode of I8253 can be confusing at best.
 * Hence, the following summary.
 *
 * =======================================================================================
 * |                               | Type     | Trigger                 | Output Form    |
 * =======================================================================================
 * |Interrupt on Terminal Count    | Oneshot  | Counter Reload          | Flip-flop like |
 * |Hardware Retriggerable Oneshot | Oneshot  | Gate Input Rising Edge  | Flip-flop like |
 * |Software Triggered Strobe      | Oneshot  | Counter Reload          | Pulse like     |
 * |Hardware Triggered Strobe      | Oneshot  | Gate Input Rising Edge  | Pulse like     |
 * |Rate Generator                 | Periodic | N.A.                    | Pulse Like     |
 * |Square Wave Generator          | Periodic | N.A.                    | Flip-flop like |
 * =======================================================================================
 *
 * Worth noting is that if the trigger of a mode is on gate input rising edge,
 * it is really on usable for channel 2 since that is the only channel for
 * which the gate input could be controlled by software. */
enum i8253_operating_mode
{
  I8253_INTERRUPT_ON_TERMINAL_COUNT    = 0b00000000,
  I8253_HARDWARE_RETRIGGERABLE_ONESHOT = 0b00000010,
  I8253_RATE_GENERATOR1                = 0b00000100,
  I8253_SQUARE_WAVE_GENERATOR1         = 0b00000110,

  I8253_SOFTWARE_TRIGGERED_STROBE      = 0b00001000,
  I8253_HARDWARE_TRIGGERED_STROBE      = 0b00001010,
  I8253_RATE_GENERATOR2                = 0b00001100,
  I8253_SQUARE_WAVE_GENERATOR2         = 0b00001110,
};

#define I8253_BCD 0b00000001;

struct i8253
{
  struct timer timer;
};

static uint64_t _i8253_reload_value_from_duration(uint64_t duration)
{
  // Duration specified in ns
  return duration * 1193192 / 1000000000;
}

static uint16_t i8253_reload_value_from_duration(unsigned duration)
{
  uint64_t reload_value = _i8253_reload_value_from_duration(duration);
  if(reload_value >= UINT16_MAX)
    reload_value = UINT16_MAX;

  if(reload_value == 0)
    reload_value = 1;

  return reload_value;
}

static void i8253_configure(struct timer *timer, enum timer_mode mode)
{
  struct i8253 *i8253 = (struct i8253 *)timer;
  switch(mode)
  {
  case TIMER_MODE_ONESHOT:
    outb(I8253_MODE_COMMAND_REGISTER, I8253_SELECT_CHANNEL0 | I8253_ACCESS_LOBYTE_HIBYTE | I8253_INTERRUPT_ON_TERMINAL_COUNT);
    break;
  case TIMER_MODE_PERIODIC:
    outb(I8253_MODE_COMMAND_REGISTER, I8253_SELECT_CHANNEL0 | I8253_ACCESS_LOBYTE_HIBYTE | I8253_SQUARE_WAVE_GENERATOR1);
    break;
  default:
    KASSERT_UNREACHABLE;
  }
}

static void i8253_reload(struct timer *timer, unsigned duration)
{
  uint16_t reload_value = i8253_reload_value_from_duration(duration);
  outb(I8253_SELECT_CHANNEL0, (reload_value >> 0) & 0xFF);
  outb(I8253_SELECT_CHANNEL0, (reload_value >> 8) & 0xFF);
}

static int i8253_init(struct i8253 *pit)
{
  pit->timer.configure = &i8253_configure;
  pit->timer.reload    = &i8253_reload;
  if(acquire_ports(THIS_MODULE, I8253_PORTS, I8253_PORT_COUNT) != 0)
    return -1;

  slot_init(&pit->timer.slot, NULL, "i8253", NULL);
  irqs_bus_set_output("isa", 0, &pit->timer.slot);
  return 0;
}

static void i8253_fini(struct i8253 *pit)
{
  irqs_bus_unset_output("isa", 0);
  KASSERT(release_ports(THIS_MODULE, I8253_PORTS, I8253_PORT_COUNT) == 0);
}

static struct i8253 i8253;
void i8253_module_init()
{
  i8253_init(&i8253);
  timer_register(&i8253.timer);
}

