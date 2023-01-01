#include "slot.h"

#include <kcore/assert.h>
#include <kcore/debug.h>

#include <stddef.h>

void irq_slot_connect(struct irq_slot *prev, struct irq_slot *next)
{
  KASSERT(prev->next == NULL);
  KASSERT(next->prev == NULL);
  prev->next = next;
  next->prev = prev;
}

void irq_slot_disconnect(struct irq_slot *prev, struct irq_slot *next)
{
  KASSERT(prev->next == next);
  KASSERT(next->prev == prev);
  prev->next = NULL;
  next->prev = NULL;
}

void irq_slot_emit(struct irq_slot *slot)
{
  for(; slot; slot = slot->next)
  {
    debug_printf("emitting on slot with name=%s\n", slot->name ? slot->name : "none");
    if(slot->ops && slot->ops->on_emit)
      if(!slot->ops->on_emit(slot))
        break;
  }
}

void irq_slot_unmask(struct irq_slot *slot)
{
  for(; slot; slot = slot->prev)
  {
    debug_printf("unmasking on slot with name=%s\n", slot->name ? slot->name : "none");
    if(slot->ops && slot->ops->on_unmask)
      slot->ops->on_unmask(slot);
  }
}

void irq_slot_mask(struct irq_slot *slot)
{
  for(; slot; slot = slot->prev)
  {
    debug_printf("masking on slot with name=%s\n", slot->name ? slot->name : "none");
    if(slot->ops && slot->ops->on_mask)
      slot->ops->on_mask(slot);
  }
}
