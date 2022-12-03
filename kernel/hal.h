#ifndef HAL_H
#define HAL_H

#include "hal/ports.h"
#include "hal/gdt.h"
#include "hal/idt.h"
#include "hal/exceptions.h"

void hal_init();

#endif // HAL_H
