#include "debug.h"

#include "core/debug.h"

#include <stdbool.h>
#include <stdint.h>

static void outb(uint16_t port, uint8_t data)
{
  asm volatile ("out %[data], %[port]" : : [port]"d"(port), [data]"a"(data));
}

static uint8_t inb(uint16_t port)
{
  uint8_t byte;
  asm volatile ("in %[port], %[byte] " : [byte]"=a"(byte) : [port]"d"(port));
  return byte;
}

static const uint16_t SERIAL_COM1_BASE          = 0x3F8;     /* COM1 base port */

static const uint16_t SERIAL_DATA_PORT          = SERIAL_COM1_BASE;

static const uint16_t SERIAL_FIFO_COMMAND_PORT  = SERIAL_COM1_BASE + 2;
static const uint16_t SERIAL_LINE_COMMAND_PORT  = SERIAL_COM1_BASE + 3;
static const uint16_t SERIAL_MODEM_COMMAND_PORT = SERIAL_COM1_BASE + 4;

static const uint16_t SERIAL_LINE_STATUS_PORT   = SERIAL_COM1_BASE + 5;

void serial_init()
{
  uint16_t divisor = 1;

  outb(SERIAL_LINE_COMMAND_PORT, 0x80);
  outb(SERIAL_DATA_PORT, (divisor >> 8) & 0xFF);
  outb(SERIAL_DATA_PORT, (divisor >> 0) & 0xFF);

  outb(SERIAL_LINE_COMMAND_PORT,  0x03);
  outb(SERIAL_FIFO_COMMAND_PORT,  0xC7);
  outb(SERIAL_MODEM_COMMAND_PORT, 0x03);
}

void debug_init()
{
  serial_init();
}

bool serial_is_transmitter_empty()
{
  return inb(SERIAL_LINE_STATUS_PORT) & 0x20;
}

void serial_put(char c)
{
  while(!serial_is_transmitter_empty());
  outb(SERIAL_DATA_PORT, c);
}

void debug_put(char c)
{
  serial_put(c);
}

void debug_write(const char *str)
{
  for(const char *p = str; *p != '\0'; ++p)
    debug_put(*p);
}
