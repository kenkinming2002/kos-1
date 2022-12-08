#include <core/slot.h>

#include <core/debug.h>

#include <stddef.h>

void slot_init(struct slot *slot, struct slot_ops *ops, const char *name, void *data)
{
  slot->ops  = ops;
  slot->name = name;
  slot->data = data;
}

void slot_emit(struct slot *slot)
{
  for(; slot; slot = slot->next)
  {
    debug_printf("emitting on slot with name=%s\n", slot->name ? slot->name : "none");
    if(slot->ops && slot->ops->on_emit)
      slot->ops->on_emit(slot);
  }
}

void slot_connect(struct slot *slot, struct slot *next)
{
  slot->next = next;
  if(slot->ops && slot->ops->on_connect)
    slot->ops->on_connect(slot);
}

void slot_disconnect(struct slot *slot)
{
  slot->next = NULL;
  if(slot->ops && slot->ops->on_disconnect)
    slot->ops->on_disconnect(slot);
}
