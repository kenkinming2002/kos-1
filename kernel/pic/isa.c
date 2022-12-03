#include "isa.h"

#include "pic8259.h"

int isa_irq_register(struct module *module, unsigned irq, handler_t handler)
{
  return pic8259s_irq_register(module, irq, handler);
}

int isa_irq_deregister(struct module *module, unsigned irq, handler_t handler)
{
  return pic8259s_irq_deregister(module, irq, handler);
}
