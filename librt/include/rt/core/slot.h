#ifndef RT_CORE_SLOT_H
#define RT_CORE_SLOT_H

#include <rt/core/ll.h>
#include <stdbool.h>

struct slot;
struct slot_ops
{
  // TODO: Consider taking void *
  void(*on_enable)(struct slot *slot);
  void(*on_disable)(struct slot *slot);
  bool(*on_emit)(struct slot *slot);
};

struct slot
{
  struct ll_node node;

  struct slot *parent;
  struct ll    childs;

  const char      *name;
  struct slot_ops *ops;
  void            *data;

  int active_count;
};

void slot_init(struct slot *slot);

void slot_activate(struct slot *slot);
void slot_deactivate(struct slot *slot);

void slot_connect(struct slot *parent, struct slot *child);
void slot_disconnect(struct slot *parent, struct slot *child);

void slot_emit(struct slot *slot);

#endif // RT_CORE_SLOT_H
