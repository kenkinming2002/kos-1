#ifndef PORTS_H
#define PORTS_H

#include "module.h"

#include <stdint.h>

int acquire_ports(struct module *module, uint16_t begin, uint16_t count);
int release_ports(struct module *module, uint16_t begin, uint16_t count);

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);
void io_wait();

#endif // PORTS_H

