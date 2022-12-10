#include "slot.h"

#include <core/assert.h>
#include <core/debug.h>

#include <stddef.h>

void irq_slot_init(struct irq_slot *slot, struct irq_slot_ops *ops, const char *name, void *data)
{
  slot->ops  = ops;
  slot->name = name;
  slot->data = data;
}

void irq_slot_emit_backward(struct irq_slot *slot)
{
  for(; slot; slot = slot->prev)
  {
    debug_printf("emitting backward on slot with name=%s\n", slot->name ? slot->name : "none");
    if(slot->ops && slot->ops->on_emit)
      slot->ops->on_emit(slot);
  }
}

void irq_slot_emit_forward(struct irq_slot *slot)
{
  for(; slot; slot = slot->next)
  {
    debug_printf("emitting forward on slot with name=%s\n", slot->name ? slot->name : "none");
    if(slot->ops && slot->ops->on_emit)
      slot->ops->on_emit(slot);
  }
}

void irq_slot_connect(struct irq_slot *prev, struct irq_slot *next)
{
  KASSERT(prev->next == NULL);
  KASSERT(next->prev == NULL);
  prev->next = next;
  next->prev = prev;
  if(prev->ops && prev->ops->on_connect_next) prev->ops->on_connect_next(prev);
  if(next->ops && next->ops->on_connect_prev) next->ops->on_connect_prev(next);
}

void irq_slot_disconnect(struct irq_slot *prev, struct irq_slot *next)
{
  KASSERT(prev->next == next);
  KASSERT(next->prev == prev);
  prev->next = NULL;
  next->prev = NULL;
  if(prev->ops && prev->ops->on_disconnect_next) prev->ops->on_disconnect_next(prev);
  if(next->ops && next->ops->on_disconnect_prev) next->ops->on_disconnect_prev(next);
}

