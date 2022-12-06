#ifndef DEV_I8259_H
#define DEV_I8259_H

#include "hal.h"

extern struct irqs_source *i8259_master;
extern struct irqs_source *i8259_slave;

void i8259_init();

#endif // DEV_I8259_H
