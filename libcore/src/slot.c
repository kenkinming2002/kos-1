#include <core/slot.h>

#include <core/assert.h>
#include <core/debug.h>

#include <stddef.h>

void slot_init(struct slot *slot, struct slot_ops *ops, const char *name, void *data)
{
  slot->ops  = ops;
  slot->name = name;
  slot->data = data;
}

void slot_emit_backward(struct slot *slot)
{
  for(; slot; slot = slot->prev)
  {
    debug_printf("emitting backward on slot with name=%s\n", slot->name ? slot->name : "none");
    if(slot->ops && slot->ops->on_emit)
      slot->ops->on_emit(slot);
  }
}

void slot_emit_forward(struct slot *slot)
{
  for(; slot; slot = slot->next)
  {
    debug_printf("emitting forward on slot with name=%s\n", slot->name ? slot->name : "none");
    if(slot->ops && slot->ops->on_emit)
      slot->ops->on_emit(slot);
  }
}

void slot_connect(struct slot *prev, struct slot *next)
{
  KASSERT(prev->next == NULL);
  KASSERT(next->prev == NULL);
  prev->next = next;
  next->prev = prev;
  if(prev->ops && prev->ops->on_connect_next) prev->ops->on_connect_next(prev);
  if(next->ops && next->ops->on_connect_prev) next->ops->on_connect_prev(next);
}

void slot_disconnect(struct slot *prev, struct slot *next)
{
  KASSERT(prev->next == next);
  KASSERT(next->prev == prev);
  prev->next = NULL;
  next->prev = NULL;
  if(prev->ops && prev->ops->on_disconnect_next) prev->ops->on_disconnect_next(prev);
  if(next->ops && next->ops->on_disconnect_prev) next->ops->on_disconnect_prev(next);
}
