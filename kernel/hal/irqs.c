#include "irqs.h"

#include <core/assert.h>

#include <stddef.h>

/* IDT */
struct irq_line
{
  struct module *module;
  irq_handler_t handler;
};

#define MAX_IRQS 256
struct irq_line lines[256];

int acquire_irq(struct module *module, unsigned irq, irq_handler_t handler)
{
  if(irq >= MAX_IRQS)
    return -1;

  if(lines[irq].module || lines[irq].handler)
    return -1;

  lines[irq].module  = module;
  lines[irq].handler = handler;
  return 0;
}

int release_irq(struct module *module, unsigned irq, irq_handler_t handler)
{
  if(irq >= MAX_IRQS)
    return -1;

  if(lines[irq].module != module || lines[irq].handler != handler)
    return -1;

  lines[irq].module  = NULL;
  lines[irq].handler = NULL;
  return 0;
}

int trigger_irq(irq_t irq)
{
  if(irq >= MAX_IRQS)
    return -1;

  if(!lines[irq].module || !lines[irq].handler)
    return -1;

  lines[irq].handler();
  return 0;
}

