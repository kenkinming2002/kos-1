#include <rt/asm/io.h>

uint8_t inb(uint16_t port)
{
  uint8_t value;
  asm volatile ("inb %1, %0" : "=a"(value) : "d"(port));
  return value;
}

void outb(uint16_t port, uint8_t value)
{
  asm volatile ("outb %0, %1" : : "a"(value), "d"(port));
}

void io_wait()
{
  outb(0x80, 0);
}


