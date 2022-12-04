#ifndef HAL_IDT_H
#define HAL_IDT_H

#include "module.h"

#define IDT_MAX_VECTOR     ((unsigned)256)
#define IDT_INVALID_VECTOR ((unsigned)-1)

void idt_init();

#endif // HAL_IDT_H
