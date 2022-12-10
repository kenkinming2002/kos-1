#ifndef HAL_IRQ_SLOT_H
#define HAL_IRQ_SLOT_H

struct irq_slot;
struct irq_slot_ops
{
  void(*on_emit)(struct irq_slot *slot);

  void(*on_connect_prev)(struct irq_slot *slot);
  void(*on_disconnect_prev)(struct irq_slot *slot);

  void(*on_connect_next)(struct irq_slot *slot);
  void(*on_disconnect_next)(struct irq_slot *slot);
};

struct irq_slot
{
  struct irq_slot_ops *ops;
  struct irq_slot *prev, *next;
  const char *name;
  void *data;
};

void irq_slot_init(struct irq_slot *slot, struct irq_slot_ops *ops, const char *name, void *data);

void irq_slot_emit_backward(struct irq_slot *slot);
void irq_slot_emit_forward(struct irq_slot *slot);

void irq_slot_connect(struct irq_slot *prev, struct irq_slot *next);
void irq_slot_disconnect(struct irq_slot *prev, struct irq_slot *next);

#endif // HAL_IRQ_SLOT_H
