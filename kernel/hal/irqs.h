#ifndef HAL_IRQS_H
#define HAL_IRQS_H

#include "module.h"

#include <core/ll.h>

int acquire_irqs(struct module *module, unsigned begin, unsigned count);
int release_irqs(struct module *module, unsigned begin, unsigned count);

#define IRQ_DOMAIN_DYNAMIC_BASE (unsigned)-1

struct irq_domain
{
  struct ll_node node;

  unsigned base;
  unsigned count;

  void(*handler)(unsigned, void*);
  void *data;
};

struct irq_domain *irq_alloc_domain(unsigned base, unsigned count);
void irq_free_domain(struct irq_domain *domain);

#endif // HAL_IRQS_H
