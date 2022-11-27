#ifndef PORTS_H
#define PORTS_H

#include "driver.h"

#include <stdint.h>

int acquire_ports(struct driver *driver, uint16_t begin, uint16_t count);
int release_ports(struct driver *driver, uint16_t begin, uint16_t count);

#endif // PORTS_H

