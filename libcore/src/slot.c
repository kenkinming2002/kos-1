#include <core/slot.h>

#include <core/assert.h>
#include <core/debug.h>

#include <stddef.h>

void slot_init(struct slot *slot)
{
  slot->parent = NULL;
  slot->childs = LL_INIT(slot->childs);

  slot->name   = NULL;
  slot->ops    = NULL;
  slot->data   = NULL;

  slot->active_count = 0;
}

static void slot_adjust(struct slot *slot, int offset)
{
  debug_printf("TRACE: slot_adjust: slot->name = %s, offset = %d\n", slot->name ? slot->name : "none", offset);
  int old_active_count = slot->active_count;
  int new_active_count = slot->active_count + offset;
  if(old_active_count == 0 && new_active_count != 0)
  {
    if(slot->ops && slot->ops->on_enable)
      slot->ops->on_enable(slot);
  }
  else if(old_active_count != 0 && new_active_count == 0)
  {
    if(slot->ops && slot->ops->on_disable)
      slot->ops->on_disable(slot);
  }

  slot->active_count = new_active_count;
}

void slot_activate(struct slot *slot)
{
  debug_printf("TRACE: slot_activate\n");
  for(; slot; slot = slot->parent)
    slot_adjust(slot, 1);
}

void slot_deactivate(struct slot *slot)
{
  debug_printf("TRACE: slot_deactivate\n");
  for(; slot; slot = slot->parent)
    slot_adjust(slot, -1);
}

void slot_connect(struct slot *parent, struct slot *child)
{
  debug_printf("TRACE: slot_connect: parent->name = %s, child->name = %s\n", parent->name ? parent->name : "none", child->name ? child->name : "none");
  child->parent = parent;
  ll_append(&parent->childs, &child->node);
}

void slot_disconnect(struct slot *parent, struct slot *child)
{
  debug_printf("TRACE: slot_disconnect: parent->name = %s, child->name = %s\n", parent->name ? parent->name : "none", child->name ? child->name : "none");
  child->parent = NULL;
  ll_delete(&child->node);
}

void slot_emit(struct slot *slot)
{
  debug_printf("TRACE: slot_emit: slot->name = %s\n", slot->name ? slot->name : "none");
  if(slot->ops && slot->ops->on_emit)
    slot->ops->on_emit(slot);

  LL_FOREACH(slot->childs, node) {
    struct slot *child = (struct slot *)node;
    slot_emit(child);
  }
}

