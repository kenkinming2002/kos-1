#ifndef HAL_IRQ_SLOT_H
#define HAL_IRQ_SLOT_H

#include <stdbool.h>

struct irq_slot;
struct irq_slot_ops
{
  void(*on_unmask)(struct irq_slot *slot);
  void(*on_mask)(struct irq_slot *slot);
  bool(*on_emit)(struct irq_slot *slot);
};

struct irq_slot
{
  struct irq_slot *prev, *next;

  const char *name;
  struct irq_slot_ops *ops;
  void *data;
};

#define IRQ_SLOT_INIT(_name, _ops, _data) (struct irq_slot){ .prev = NULL, .next = NULL, .name = _name, .ops = _ops, .data = _data, }

void irq_slot_connect(struct irq_slot *prev, struct irq_slot *next);
void irq_slot_disconnect(struct irq_slot *prev, struct irq_slot *next);

// Propagate upstream
void irq_slot_unmask(struct irq_slot *slot);
void irq_slot_mask(struct irq_slot *slot);

// Propagate downstream
void irq_slot_emit(struct irq_slot *slot);

#endif // HAL_IRQ_SLOT_H
