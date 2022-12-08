#ifndef HAL_IRQS_H
#define HAL_IRQS_H

#include "module.h"

#include <core/ll.h>
#include <core/slot.h>

typedef void(*handler_t)(void *);
struct irqs_source
{
  void(*set_base)(struct irqs_source *source, unsigned base);

  void(*acknowledge)(struct irqs_source *source);
  void(*unmask)(struct irqs_source *source, unsigned irq);
  void(*mask)(struct irqs_source *source, unsigned irq);
};

int acquire_irqs(struct module *module, unsigned begin, unsigned count, struct irqs_source *source);
int release_irqs(struct module *module, unsigned begin, unsigned count, struct irqs_source *source);

int irqs_register_handler(struct irqs_source *source, struct module *module, unsigned irq, handler_t handler, void *data);
int irqs_deregister_handler(struct irqs_source *source, struct module *module, unsigned irq);

extern struct slot irq_slots[];

#endif // HAL_IRQS_H
