#include "irqs.h"

#include <stddef.h>

struct irq_line
{
  struct driver *driver;
  irq_handler_t handler;
};

#define MAX_IRQS 32
struct irq_line lines[MAX_IRQS];

int acquire_irq(struct driver *driver, unsigned irq, irq_handler_t handler)
{
  if(irq >= MAX_IRQS)
    return -1;

  if(lines[irq].driver || lines[irq].handler)
    return -1;

  lines[irq].driver  = driver;
  lines[irq].handler = handler;
  return 0;
}

int release_irq(struct driver *driver, unsigned irq, irq_handler_t handler)
{
  if(irq >= MAX_IRQS)
    return -1;

  if(lines[irq].driver != driver || lines[irq].handler != handler)
    return -1;

  lines[irq].driver  = NULL;
  lines[irq].handler = NULL;
  return 0;
}

int trigger_irq(unsigned irq)
{
  if(!lines[irq].driver || !lines[irq].handler)
    return -1;

  lines[irq].handler();
  return 0;
}
