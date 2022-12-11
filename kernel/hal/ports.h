#ifndef PORTS_H
#define PORTS_H

#include "module.h"

#include <stdint.h>

int acquire_ports(struct module *module, uint16_t begin, uint16_t count);
int release_ports(struct module *module, uint16_t begin, uint16_t count);

#endif // PORTS_H

