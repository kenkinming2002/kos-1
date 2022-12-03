#ifndef TIMER_PIT_H
#define TIMER_PIT_H

#include "module.h"

typedef void(*timer_callback_t)();

void pit_init();
int pit_register_callback(struct module *module, timer_callback_t callback);
int pit_deregister_callback(struct module *module, timer_callback_t callback);

#endif // TIMER_PIT_H
