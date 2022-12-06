#include "isa.h"

#include "dev/pic8259.h"

#include <stdbool.h>
#include <core/assert.h>

int isa_irq_register(struct module *module, unsigned irq, handler_t handler, void *data)
{
  struct irqs_source *pic;
  if(irq < 8)
    pic = pic8259_master;
  else if(irq < 16)
    pic = pic8259_slave;
  else
    KASSERT_UNREACHABLE;

  irq %= 8;

  return irqs_register_handler(pic, module, irq, handler, data);
}

int isa_irq_deregister(struct module *module, unsigned irq)
{
  struct irqs_source *pic;
  if(irq < 8)
    pic = pic8259_master;
  else if(irq < 16)
    pic = pic8259_slave;
  else
    KASSERT_UNREACHABLE;

  irq %= 8;

  return irqs_deregister_handler(pic, module, irq);
}

