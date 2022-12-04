#ifndef HAL_IRQS_H
#define HAL_IRQS_H

#include "module.h"

#include <core/ll.h>

int acquire_irqs(struct module *module, unsigned begin, unsigned count);
int release_irqs(struct module *module, unsigned begin, unsigned count);

typedef void(*handler_t)(void);
struct irqs_source
{
  void(*set_base)(struct irqs_source *source, unsigned base);

  void(*acknowledge)(struct irqs_source *source, unsigned irq);
  void(*unmask)(struct irqs_source *source, unsigned irq);
  void(*mask)(struct irqs_source *source, unsigned irq);
};
int irqs_attach_source(struct module *module, unsigned begin, unsigned count, struct irqs_source *source);
int irqs_detach_source(struct module *module, unsigned begin, unsigned count, struct irqs_source *source);

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
