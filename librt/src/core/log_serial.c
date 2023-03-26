#include <rt/core/once.h>
#include <rt/asm/io.h>

#include <stdbool.h>
#include <stdint.h>

static const uint16_t SERIAL_COM1_BASE          = 0x3F8;     /* COM1 base port */

static const uint16_t SERIAL_DATA_PORT          = SERIAL_COM1_BASE;

static const uint16_t SERIAL_FIFO_COMMAND_PORT  = SERIAL_COM1_BASE + 2;
static const uint16_t SERIAL_LINE_COMMAND_PORT  = SERIAL_COM1_BASE + 3;
static const uint16_t SERIAL_MODEM_COMMAND_PORT = SERIAL_COM1_BASE + 4;

static const uint16_t SERIAL_LINE_STATUS_PORT   = SERIAL_COM1_BASE + 5;

static void serial_init()
{
  uint16_t divisor = 1;

  outb(SERIAL_LINE_COMMAND_PORT, 0x80);
  outb(SERIAL_DATA_PORT, (divisor >> 8) & 0xFF);
  outb(SERIAL_DATA_PORT, (divisor >> 0) & 0xFF);

  outb(SERIAL_LINE_COMMAND_PORT,  0x03);
  outb(SERIAL_FIFO_COMMAND_PORT,  0xC7);
  outb(SERIAL_MODEM_COMMAND_PORT, 0x03);
}

static bool serial_is_transmitter_empty()
{
  return inb(SERIAL_LINE_STATUS_PORT) & 0x20;
}

static void serial_transmit(char c)
{
  while(!serial_is_transmitter_empty());
  outb(SERIAL_DATA_PORT, c);
}

void arch_logc(char c)
{
  static struct once once;
  if(once_begin(&once, ONCE_SYNC))
  {
    serial_init();
    once_end(&once, ONCE_SYNC);
  }
  serial_transmit(c);
}

