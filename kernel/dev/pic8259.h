#ifndef DEV_PIC8259_H
#define DEV_PIC8259_H

#include "hal.h"

extern struct irqs_source *pic8259_master;
extern struct irqs_source *pic8259_slave;

void pic8259_init();

#endif // DEV_PIC8259_H
