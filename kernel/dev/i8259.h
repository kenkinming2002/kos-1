#ifndef DEV_I8259_H
#define DEV_I8259_H

#include "hal.h"

void i8259_module_init();

extern struct slot *i8259_master_slots;
extern struct slot *i8259_slave_slots;

#endif // DEV_I8259_H
